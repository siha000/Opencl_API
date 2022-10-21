Opencl_API
======
```
cl_int clGetPlatformIDs( cl_uint num_entries, 
                         cl_platform_id *platforms, 
                         cl_uint *num_platforms)
OpenCL 使用 cl_platform_id 表示平台，通過 clGetPlatformIDs 獲取可使用的平台數組。當平台數未知，第一次調用 clGetPlatformIDs 獲取平台個數，第二次調用 clGetPlatformIDs 獲取平台對象。

```