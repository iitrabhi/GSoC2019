// the OOP version in C++
// https://gist.github.com/darkstalker/eeb7e48a45f1b78db4a2c6ebfd01e926
#include <iostream>

// base abstract class. that is what we use as the interface
class Animal
{
public:
    Animal(char const* name): m_name(name) {}

    // this is required to properly delete virtual classes
    virtual ~Animal() {}

    // with this strange syntax we define an unimplemeted "interface" function
    virtual void make_sound() = 0;

protected:
    // shared data field
    std::string m_name;
};

// derived class. that means that Dog is a more refined version of Animal
// you usually inherit from only a single base class
class Dog: public Animal
{
public:
    // need to forward the constructor arguments
    Dog(char const* name): Animal(name) {}

    // here we implement the interface
    void make_sound() override
    {
        std::cout << m_name << " the dog said: bork!" << std::endl;
    }

    // type-specific method
    void wag()
    {
        std::cout << "*" << m_name << " wags*" << std::endl;
    }
};

// same as above, but with a different implementation
class Cat: public Animal
{
public:
    Cat(char const* name): Animal(name) {}

    void make_sound() override
    {
        std::cout << m_name << " the cat said: mow!" << std::endl;
    }

    void purr()
    {
        std::cout << "*" << m_name << " purrs*" << std::endl;
    }
};

// We can now use Animal as the general type
// There is runtime type information that allows this (dynamic dispatch)
void animal_sound(Animal& a)
{
    // method called via the generic interface
    a.make_sound();

    // type information is still there, we can extract the original type by
    // probing if dynamic cast works
    auto d = dynamic_cast<Dog*>(&a);
    if (d != nullptr)
        d->wag();

    auto c = dynamic_cast<Cat*>(&a);
    if (c != nullptr)
        c->purr();
}

// we can do static dispatch too, via templates (duck typing)
template <typename T>
void animal_sound_1(T& a)
{
    a.make_sound();
    // however, we can't easily access type information here without
    // using specialization..
}

// ..and that's pretty much equivalent to overloading the function
// this works by making two different functions with internally mangled names
// for each argument combination
void animal_sound_2(Dog& a) { a.make_sound(); a.wag(); }
void animal_sound_2(Cat& a) { a.make_sound(); a.purr(); }

int main()
{
    auto cat = Cat("kitty");
    auto dog = Dog("puppy");
    animal_sound(cat);
    animal_sound(dog);
    animal_sound_1(cat);
    animal_sound_1(dog);
    animal_sound_2(cat);
    animal_sound_2(dog);
    return 0;
}