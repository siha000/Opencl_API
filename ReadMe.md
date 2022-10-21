Opencl_API
======

### cl_int clGetPlatformIDs

```
cl_int clGetPlatformIDs( cl_uint num_entries, 
                         cl_platform_id *platforms, 
                         cl_uint *num_platforms)

OpenCL 使用 cl_platform_id 表示平台，通過 clGetPlatformIDs
獲取可使用的平台數組。當平台數未知，第一次調用 clGetPlatformIDs 獲取平台個數，
第二次調用 clGetPlatformIDs 獲取平台對象。


參數說明：
- num_entries [IN] ：要獲取的平台數量，如果platforms非空，則num_entries不能為0。
- platforms [OUT] ：返回獲取的平台對像數組。
-num_platforms [OUT] ：用於查詢返回可用的平台數目，num_platforms可設為NULL 忽略
- Return ：正常執行返回CL_SUCCESS

```

### clGetPlatformInfo

```
cl_int clGetPlatformInfo( cl_platform_id platform, 
                          cl_platform_info param_name, 
                          size_t param_value_size, 
                          void *param_value, 
                          size_t *param_value_size_ret)

獲取平台相關信息，如OpenCL 配置版本。


參數說明：
- platform [IN] ：查詢的平台對象。
- param_name [IN] ：表示平台查詢參數的枚舉常量，參考表2-1。
- param_value_size [IN] ：param_value指向內存空間的字節數。
- param_value [OUT] ：指向返回查詢參數結果的內存指針。
- param_value_size_ret [OUT] ：返回查詢參數的實際長度。
- Return ：正常執行返回CL_SUCCESS

```

### clGetDeviceIDs

```
cl_int clGetDeviceIDs (cl_platform_id platform, 
                       cl_device_type device_type, 
                       cl_uint num_entries, 
                       cl_device_id *devices, 
                       cl_uint *num_devices)

獲取平台可使用的Device 對像數組

參數說明：
- platform [IN] ：clGetPlatformIDs獲取的Platform ID。
- device_type [IN] ：獲取OpenCL Device 的類型，參考表2-2。
- num_entries [IN] ：要獲取的設備數量。
- devices [OUT] ：返回獲取的設備對像數組。
- num_devices [OUT]：返回平台連接device_type類型設備數目，可設為NULL 忽略。
- Return ：正常執行返回CL_SUCCESS 


```

### clGetDeviceInfo

```
cl_int clGetDeviceinfo(cl_device_id device,
                       cl_device_info param_name,
                       size_t param_value_size,
                       void *param_value,
                       size_t *param_value_size_ret)


獲取設備相關信息，例如並行計算單元數，全局內存大小等等。

參數說明：
- device [IN] ：clGetDeviceIDs獲取的Device ID。
- param_name [IN] ：表示設備查詢參數的枚舉常量。
- param_value_size [IN] ：param_value指向內存空間的字節數。
- param_value [OUT] ：指向返回查詢參數結果的內存指針。
- param_value_size_ret [OUT]：返回查詢參數的實際長度。
- Return ：正常執行返回CL_SUCCESS
```