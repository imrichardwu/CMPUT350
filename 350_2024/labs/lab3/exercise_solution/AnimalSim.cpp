#include <cassert>
#include <cstddef>
#include <iostream>
#include <random>

struct World;    // forward declaration - World& needed in Animal

// animal base-class

struct Animal {
    // implemented ...

    Animal(int x, int y)
        : x(x), y(y) {}

    // act in the world
    virtual void act(World &w) = 0;

    // prints animal data
    virtual void print() const = 0;

    virtual ~Animal() = default;    // Defaulted destructor, same as virtual ~Animal() {}

    // sends type and common data to stdout
    void print_common(const char *type) const {
        std::cout << type << " " << x << " " << y << " " << age << std::endl;
    }

    // data
    int x, y;             // animal's location
    bool alive = true;    // true iff animal is alive
    int age = 0;          // animal's age
};

// simulation environment

struct World {
    // ONLY CHANGE CODE IN BLOCKS DESIGNATED WITH "implement ..." !

    // init_cap: make room for that many animals initially
    World(int init_cap = 1)
        : rng(0), capacity(init_cap) {
        assert(init_cap > 0);
        n = 0;
        op = A = new Animal *[capacity];
    }

    ~World() {
        for (int i = 0; i < n; ++i) {
            delete A[i];
        }
        delete[] A;
    }

    //  CC and AO not implemented
    World(const World &) = delete;
    World &operator=(const World &) = delete;

    // return number of animals
    int animal_number() const {
        return n;
    }

    // return pointer to random animal
    Animal *get_random_animal() {
        assert(n > 0);
        // Create a uniform distribution object, which samples in the inclusive range given
        // While creating an object every iteration might seem expensive, compilers should optimize this out
        // This may not be the case for stateful distributions, so always measure!
        std::uniform_int_distribution<std::size_t> dist(0, static_cast<std::size_t>(n) - 1);
        return A[dist(rng)];
    }

    // add animal to array (world now owns it)
    // assumes there is enough space left
    void add_animal(Animal *p) {
        assert(op < A + capacity);
        *op++ = p;
    }

    // print world state (list of animals)
    void print() const {
        // implemented ...

        std::cout << " " << n << " animal(s)" << std::endl;

        for (int i = 0; i < n; ++i) {
            std::cout << " ";    // Leading space
            A[i]->print();
        }
    }

    // execute 'steps' action rounds
    void simulate(int steps) {
        remove_dead_animals();    // also sets n

        // Print number of animals, then each individual animal
        // Need to do this before we step the world forward
        std::cout << "iter 0" << std::endl;
        print();

        for (int i = 0; i < steps && n > 0; ++i) {
            // allocate sufficient space for new offspring
            make_room_for_offspring();

            // all animals act and have their age incremented

            // implemented ...

            for (int j = 0; j < n; ++j) {
                A[j]->act(*this);
                ++(A[j]->age);
            }

            // finally, remove dead animals
            remove_dead_animals();

            // Print number of animals, then each individual animal
            // Loop iteration i is world step i+1
            std::cout << "iter " << i + 1 << std::endl;
            print();
        }
    }

private:
    // helper functions and data members

    // these are implementation details you don't have to understand

    // reserve enough space for the maximum possible number of offspring animals
    // created in one iteration
    void make_room_for_offspring() {
        if (capacity < 2 * n) {
            // we need more space: allocate larger array and copy animal pointers
            // over

            capacity = 2 * n;

            Animal **B = new Animal *[capacity];

            for (int i = 0; i < n; ++i) {
                B[i] = A[i];
            }

            delete[] A;
            A = B;
        }

        op = A + n;    // where offspring is stored, initially points right behind animals
    }

    // remove all dead animals
    void remove_dead_animals() {
        for (Animal **p = A; p < op; ++p) {
            if (!(*p)->alive) {
                delete *p;
                --op;
                *p = *op;    // copy last element
                --p;         // recheck same position
            }
        }

        // update number of animals
        n = static_cast<int>(op - A);
    }

    // data members

    std::mt19937 rng;    // C++11 source of randomness
    Animal **A;          // array of animal pointers (owner)
    int n;               // number of animals stored in A (can be < capacity)
    int capacity;        // number of elements in A
    Animal **op;         // where to store offspring pointers, gets incemented after
                         // adding a new animal  
};

// implemented ... Mouse, Goose, Rabbit, Bear here

struct Mouse : public Animal {
    Mouse(int x, int y)
        : Animal(x, y) {}

    void act([[maybe_unused]] World &w) override
    // "override indicates that we want to override a virtual function
    // if print() is not a virtual function in the base-class the compiler
    // will complain => safety feature
    {
        --x;    // move west
    }

    void print() const override {
        print_common("Mouse");
    }
};

struct Goose : public Animal {
    Goose(int x, int y)
        : Animal(x, y) {}

    void act([[maybe_unused]] World &w) override {
        ++y;    // fly north ...
    }

    void print() const override {
        print_common("Goose");
    }
};

struct Rabbit : public Animal {
    Rabbit(int x, int y)
        : Animal(x, y) {}

    void act(World &w) override {
        // create offspring
        w.add_animal(new Rabbit(x, y));
    }

    void print() const override {
        print_common("Rabbit");
    }
};

struct Bear : public Animal {
    Bear(int x, int y)
        : Animal(x, y) {}

    void act(World &w) override {
        // kill random animal
        // note: in a more realistic simulation, we would check proximity

        if (w.animal_number() > 1) {
            // try until another animal is killed
            for (;;) {
                Animal *v = w.get_random_animal();
                if (v != this) {    // no suicide
                    v->alive = false;
                    break;
                }
            }
        }
    }

    void print() const override {
        print_common("Bear");
    }
};

int main() {
    /* test code:

       populate world with some animals
       and simulate a few steps
    */

#if 1

    // populate world with random animals

    // a few random animals at (i, i) locations

    int N = 30;
    World w(N);

    // Source of rng
    constexpr int NUM_ANIMALS = 4;
    std::mt19937 rng(0);
    std::uniform_int_distribution<std::size_t> dist(0, static_cast<std::size_t>(NUM_ANIMALS) - 1);

    for (int i = 0; i < N; ++i) {
        // note: only approximates uniform distribution (better use C++11 rng
        // facility)

        switch (dist(rng)) {
        case 0:
            w.add_animal(new Mouse(i, i));
            break;
        case 1:
            w.add_animal(new Goose(i, i));
            break;
        case 2:
            w.add_animal(new Rabbit(i, i));
            break;
        case 3:
            w.add_animal(new Bear(i, i));
            break;
        }
    }

    // run a few steps

    w.simulate(5);

#else

    //  problems.txt test case

    // a few animals

    World w(6);

    w.add_animal(new Mouse(100, 100));
    w.add_animal(new Mouse(50, 60));
    w.add_animal(new Goose(90, 80));
    w.add_animal(new Goose(10, 20));
    w.add_animal(new Rabbit(80, 80));
    w.add_animal(new Bear(70, 90));

    // run a few steps

    w.simulate(20);

#endif

    return 0;
}
