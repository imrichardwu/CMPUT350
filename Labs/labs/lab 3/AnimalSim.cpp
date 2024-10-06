#include <cassert>
#include <cstddef>
#include <iostream>
#include <random>

struct World;    // forward declaration - World& needed in Animal

// Animal base-class

struct Animal {
    // implement ...
    int x, y;    // position
    int age;     // age
    bool alive;  // alive or dead

    Animal(int x, int y)
        : x(x), y(y), age(0), alive(true) {
    }

    virtual ~Animal(){}
    
    virtual void print() const = 0;
    virtual void act(World &w) = 0;
    void increment_age() {
        age++;
    };
};

// simulation environment

struct World {
    // ONLY CHANGE CODE IN BLOCKS DESIGNATED WITH "implement ..." !

    // init_cap: make room for that many animals initially
    World(int init_cap = 1)
        : rng(0), capacity(init_cap) {
        assert(capacity > 0);
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

    // add new animal to array (world now owns it)
    void add_animal(Animal *p) {
        assert(op < A + capacity);
        *op++ = p;
    }

    // print world state (list of animals)
    void print() const {
        std::cout << n << " animal(s)" << std::endl;
        for (int i = 0; i < n; ++i) {
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

            // implement ...

            // All animals act
            for (int i = 0; i < n; ++i) {
                if (A[i]->alive) {
                    A[i]->act(*this);
                }
            }

            // Increment age of all animals
            for (int i = 0; i < n; ++i) {
                A[i]->increment_age();
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

    // data
    std::mt19937 rng;    // C++11 source of randomness
    Animal **A;          // array of animal pointers (owner)
    int n;               // number of animals stored in A (can be < capacity)
    int capacity;        // number of elements in A
    Animal **op;         // where to store offspring pointers, gets incemented after
                         // adding a new animal  
};

// implement ... Mouse, Goose, Rabbit, Bear here
struct Mouse : public Animal {
    Mouse(int x_init, int y_init) : Animal(x_init, y_init) {}

    void act([[maybe_unused]] World &w) override {
        x -= 1;
    }

    void print() const override {
        std::cout << "Mouse " << x << " " << y << " " << age << std::endl;
    }
};

struct Goose : public Animal {
    Goose(int x_init, int y_init) : Animal(x_init, y_init) {}

    void act([[maybe_unused]] World &w) override {
        y += 1;
    }

    void print() const override {
        std::cout << "Goose " << x << " " << y << " " << age << std::endl;
    }
};

struct Rabbit : public Animal {
    Rabbit(int x_init, int y_init) : Animal(x_init, y_init) {}

    void act(World &w) override {
        w.add_animal(new Rabbit(x, y));
    }

    void print() const override {
        std::cout << "Rabbit " << x << " " << y << " " << age << std::endl;
    }
};

struct Bear : public Animal {
    Bear(int x_init, int y_init) : Animal(x_init, y_init) {}

    void act(World &w) override {
        if (w.animal_number() <= 1) {
            // Only this bear exists, cannot kill itself
            return;
        }

        // Select a random target
        Animal *target = w.get_random_animal();
        while (target == this) {
            target = w.get_random_animal();
        }

        // Kill the target
        target->alive = false;
    }

    void print() const override {
        std::cout << "Bear " << x << " " << y << " " << age << std::endl;
    }
};

int main() {
    /* test code:

       populate world with some animals and simulate a few steps
    */

    // implement ...

    World w(10);

    w.add_animal(new Mouse(100, 100));
    w.add_animal(new Mouse(50, 60));
    w.add_animal(new Goose(90, 80));
    w.add_animal(new Goose(10, 20));
    w.add_animal(new Rabbit(80, 80));
    w.add_animal(new Bear(70, 90));

    w.simulate(5);
    
    return 0;
}