__kernel void main(__global int *values) {
    values[get_global_id(0)] = get_global_id(0);
}
