
#include <stdint.h>
#include <Python.h>


typedef struct component_methods_t {
    // Convert python dictionary to hyperparameters struct
    void* (*parameters)(PyObject *dict);
    // Allocate component capsule
    void* (*create)(void *params);
    // Destroy component capsule
    void (*destroy)(void *component);
    // Get size
    void get_size(void *component);
    // Add point
    void add(void *component, void *params, float *point);
    // Remove point
    void remove(void *component, void *params, float *point);
    // Marginal Log Likelihood Ratio log(m(x_c+j)/m(x_c))
    double loglik_ratio(void *component, void *params, float *point);
    // Unconditional Log Likelihood log(m(x_j))
    double loglik_new(void *params, float *point);
} ComponentMethods;


typedef struct model_methods_t {
    // Assignment weights for existing clusters
    double log_coef(void *params, int n);
    // Assignment weights for new clusters
    double log_coef_new(void *params);

} ModelMethods;


struct components_t {
    uint32_t mem_size;
    uint32_t size;
    void *values;
}

