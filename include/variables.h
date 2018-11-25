#ifndef VARIABLES_HEADER
#define VARIABLES_HEADER

#include <string>
#include <unordered_map>
//we need the parser header for the variable type tokens.  Could just create
//more but then we're making them all over the place.  We should probably make
//out own token module or header that the parser uses so we can consolidate it
//all to one place.  I don't like the idea that the parser header is a
//dependency for alot of things
#include "parser.tab.h"

struct VariableValue {
    int var_type;
    bool initialized; //has this variable been given a value?
    unsigned int element_num;
    union {
        long i_val;
        double f_val;
        char * s_val;
    };
};

struct VariableTuple {
    std::string var_name;
    VariableValue value = {.var_type = 0};

    VariableTuple(std::string name, int type){
        this->var_name = name;
        this->value.var_type = type;
        this->value.initialized = false;
    }
    VariableTuple(std::string name, int type, long value){
        this->var_name = name;
        this->value.var_type = NUMBER;
        this->value.i_val = value;
        this->value.initialized = true;
    }
    VariableTuple(std::string name, int type, double value) {
        this->var_name = name;
        this->value.var_type = REAL;
        this->value.f_val = value;
        this->value.initialized = true;
    }
    VariableTuple(std::string name, int type, char* value) {
        this->var_name = name;
        this->value.var_type = STRING;
        this->value.s_val = value;
        this->value.initialized = true;
    }
    ~VariableTuple(){
        if(value.var_type == STRING){
            free(value.s_val);
        }
    }
};

class VariableTable {
    private:
        unsigned int num_elements = 0;
        std::unordered_map<std::string, VariableValue> table;

        VariableTable () {};

        bool check_type(int expected, int actual);
        bool check_existance(std::string name);

    public:
        unsigned int add_element(VariableTuple val);
        unsigned int add_element(std::string name, int type);
        unsigned int add_element(std::string name, int type, long value);
        unsigned int add_element(std::string name, int type, double value);
        unsigned int add_element(std::string name, int type, char * value);

        unsigned int number_elements();

        int update_element(std::string name, long value);
        int update_element(std::string name, double value);
        int update_element(std::string name, char * value);
};

#endif
