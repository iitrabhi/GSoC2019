#https://rabernat.github.io/research_computing/parallel-programming-with-mpi-for-python.html
from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
print('My rank is ',rank)