#include "poisson.h"
#include <cfloat>
#include <dolfin.h>
#include <dolfin/mesh/Ordering.h>

using namespace dolfin;

int main(int argc, char* argv[])
{
  common::SubSystemsManager::init_logging(argc, argv);
  common::SubSystemsManager::init_petsc(argc, argv);

  // Create mesh and function space
  std::array<Eigen::Vector3d, 2> pt{Eigen::Vector3d(0.0, 0.0, 0.0),
                                    Eigen::Vector3d(1.0, 1.0, 0.0)};
  auto mesh = std::make_shared<mesh::Mesh>(generation::RectangleMesh::create(
      MPI_COMM_WORLD, pt, {{32, 32}}, mesh::CellType::Type::triangle,
      mesh::GhostMode::none));

  mesh::Ordering::order_simplex(*mesh);

  ufc_function_space* space = poisson_functionspace_create();
  ufc_dofmap* ufc_map = space->create_dofmap();
  ufc_finite_element* ufc_element = space->create_element();
  auto V = std::make_shared<function::FunctionSpace>(
      mesh, std::make_shared<fem::FiniteElement>(*ufc_element),
      std::make_shared<fem::DofMap>(fem::create_dofmap(*ufc_map, *mesh)));
  std::free(ufc_element);
  std::free(ufc_map);
  std::free(space);

  auto u0 = std::make_shared<function::Function>(V);

  std::vector<std::shared_ptr<const fem::DirichletBC>> bc = {
      std::make_shared<fem::DirichletBC>(V, u0, [](auto x, bool only_boundary) {
        return (x.col(0) < DBL_EPSILON or x.col(0) > 1.0 - DBL_EPSILON);
      })};

  // Define variational forms
  ufc_form* form_a = poisson_bilinearform_create();
  auto a = std::make_shared<fem::Form>(fem::create_form(*form_a, {V, V}));
  std::free(form_a);

  ufc_form* form_L = poisson_linearform_create();
  auto L = std::make_shared<fem::Form>(fem::create_form(*form_L, {V}));
  std::free(form_L);

  auto f = std::make_shared<function::Function>(V);
  auto g = std::make_shared<function::Function>(V);

  // Attach 'coordinate mapping' to mesh
  auto cmap = a->coordinate_mapping();
  mesh->geometry().coord_mapping = cmap;

  // auto dx = Eigen::square(x - 0.5);
  // values = 10.0 * Eigen::exp(-(dx.col(0) + dx.col(1)) / 0.02);
  f->interpolate([](auto values, auto x) {
    auto dx = Eigen::square(x - 0.5);
    values = 10.0 * Eigen::exp(-(dx.col(0) + dx.col(1)) / 0.02);
  });
  g->interpolate(
      [](auto values, auto x) { values = Eigen::sin(5 * x.col(0)); });
  L->set_coefficients({{"f", f}, {"g", g}});

  // Compute solution
  function::Function u(V);
  la::PETScMatrix A = fem::create_matrix(*a);
  la::PETScVector b(*L->function_space(0)->dofmap()->index_map());

  MatZeroEntries(A.mat());
  dolfin::fem::assemble_matrix(A.mat(), *a, bc);
  MatAssemblyBegin(A.mat(), MAT_FINAL_ASSEMBLY);
  MatAssemblyEnd(A.mat(), MAT_FINAL_ASSEMBLY);

  VecSet(b.vec(), 0.0);
  VecGhostUpdateBegin(b.vec(), INSERT_VALUES, SCATTER_FORWARD);
  VecGhostUpdateEnd(b.vec(), INSERT_VALUES, SCATTER_FORWARD);
  dolfin::fem::assemble_vector(b.vec(), *L);
  dolfin::fem::apply_lifting(b.vec(), {a}, {{bc}}, {}, 1.0);
  VecGhostUpdateBegin(b.vec(), ADD_VALUES, SCATTER_REVERSE);
  VecGhostUpdateEnd(b.vec(), ADD_VALUES, SCATTER_REVERSE);
  dolfin::fem::set_bc(b.vec(), bc, nullptr);

  la::PETScKrylovSolver lu(MPI_COMM_WORLD);
  la::PETScOptions::set("ksp_type", "preonly");
  la::PETScOptions::set("pc_type", "lu");
  lu.set_from_options();

  lu.set_operator(A.mat());
  lu.solve(u.vector().vec(), b.vec());

  // Save solution in VTK format
  io::XDMFFile file(MPI_COMM_WORLD,"u.xdmf");
  file.write(u);

  return 0;
}
