#ifndef ENGINEPROTOTYPE_COMPONENT
#define ENGINEPROTOTYPE_COMPONENT

#include <string>
#include <functional>

class Component {
    public:
        static int NEXT_COMPONENT; 

        int id; 
        std::string name;
        virtual void update(float& deltaTime);
        virtual void debug();
        Component(); 
};

#endif
