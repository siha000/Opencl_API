__kernel void MatMul_2D(__global char *input, __global char *output, int height,
                     int width) {
  unsigned int row = get_global_id(1);
  unsigned int col = get_global_id(0);

  char temp = (-1) * input[(col - 1) * width + row - 3] +
              (-2) * input[(col)*width + row - 3] +
              (-1) * input[(col + 1) * width + row - 3] +
              input[(col - 1) * width + row + 3] +
              2 * input[(col)*width + row + 3] +
              input[(col + 1) * width + row + 3];

  char temp1 = (-1) * input[(col - 1) * width + row - 3] +
               (-2) * input[(col - 1) * width + row] +
               (-1) * input[(col - 1) * width + row + 3] +
               input[(col + 1) * width + row - 3] +
               2 * input[(col + 1) * width + row] +
               input[(col + 1) * width + row + 3];

  output[col * width + row] =
      abs(temp) + abs(temp1) > 255 ? (char)255 : (char)abs(temp) + abs(temp1);
}

__kernel void MatMul_1D(__global char *input, __global char *output, int width,
                     int height) {

  int col = get_global_id(0);
  char temp = (-1) * input[(col - 3) - height] + (-2) * input[(col - 3)] +
              (-1) * input[(col - 3) + height] + input[(col + 3) - height] +
              2 * input[(col + 3)] + input[(col + 3) + height];

  char temp1 = (-1) * input[(col - 3) - height] + (-2) * input[(col)-height] +
               (-1) * input[(col - 3) - height] + input[(col - 3) + height] +
               2 * input[(col) + height] + input[(col + 3) + height];
  output[col] =
      abs(temp) + abs(temp1) > 255 ? (char)255 : (char)abs(temp) + abs(temp1);
}