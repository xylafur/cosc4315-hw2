#include "variables.h"

/*  This module is just a sort of API for our VariableTable type.  The
 *  VariableTable type is really just a wrapper for a hash map that we use to
 *  store the values of variables.
 *
 *  The value in our map is a VariableValue struct, the reason there are so many
 *  methods for this class is because we automatically construct that type from
 *  the input parameters and then insert it into the table.
 */

unsigned int VariableTable::add_element (VariableTuple val){
    val.value.element_num=this->num_elements;
    this->table[val.var_name] = val.value;
    return this->num_elements++;
}

unsigned int VariableTable::add_element(std::string name, int type){
    VariableValue temp = {.var_type=type, .initialized=false,
                          .element_num=this->num_elements, .i_val=0};
    this->table[name] = temp;
    return this->num_elements++;
}

unsigned int VariableTable::add_element(std::string name, int type, long value){
    VariableValue temp = {.var_type=type, .initialized=true,
                          .element_num=this->num_elements, .i_val=value};
    this->table[name] = temp;    
    return this->num_elements++;
}

unsigned int VariableTable::add_element(std::string name, int type,
                                        double value){
    VariableValue temp = {.var_type=type,.initialized=true,
                          .element_num=this->num_elements, .f_val=value};
    this->table[name] = temp;
    return this->num_elements++;
}
unsigned int VariableTable::add_element(std::string name, int type,
                                        char * value){
    VariableValue temp = {.var_type=type, .initialized=true,
                          .element_num=this->num_elements, .s_val=value};
    this->table[name] = temp;
    return this->num_elements++;
}

unsigned int VariableTable::number_elements(){
    return this->num_elements;
}


/*  These functinos return -1 in the case where the variable does not already
 *  exist or if it does exist but is being updated with the incorrect type
 */

bool VariableTable::check_type(int expected, int actual){
    return expected == actual;
}

//return 1 if the element exists in the table
bool VariableTable::check_existance(std::string name){
    return ! (this->table.find(name) == this->table.end());
}

#define check_update(name, __type, temp, value)                     \
    if(!this->check_existance(name)){return -1;}                    \
    VariableValue temp = this->table[name];                         \
    if(!this->check_type(__type, temp.var_type)){return -2;}

int VariableTable::update_element(std::string name, long value){
    check_update(name, NUMBER, temp, value);
    temp.i_val = value;
    temp.initialized = true;
    this->table[name] = temp;

    return (int) temp.element_num;
}

int VariableTable::update_element(std::string name, double value){
    check_update(name, REAL, temp, value);
    temp.f_val = value;
    temp.initialized = true;
    this->table[name] = temp;

    return (int) temp.element_num;
}

int VariableTable::update_element(std::string name, char * value){
    check_update(name, STRING, temp, value);
    temp.s_val = value;
    temp.initialized = true;
    this->table[name] = temp;

    return (int) temp.element_num;
}
