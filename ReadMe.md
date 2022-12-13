OpenCL
======

### 安裝Intel OpenCL

+ 安裝完成後，所在目錄默認路徑為：C:\Program Files (x86)\IntelSWTools\system_studio_2020\OpenCL\sdk

+ 在C:\Program Files (x86)\IntelSWTools\system_studio_2020\OpenCL\sdk\include裡面資料夾複製到Mingw的include

+ C:\Program Files (x86)\IntelSWTools\system_studio_2020\OpenCL\sdk\lib\x86裡面資料複製到Mingw的lib

+ 執行時輸入g++ -g 檔案名稱.cpp -lOpenCL -o 檔案名稱

### OpenCL API執行流程

```
clGetPlatformIDs第一次調用獲取平台個數，第二次調用 clGetPlatformIDs 獲取平台對象。

clGetDeviceIDs第一次調用獲取設備個數，第二次調用 clGetDeviceIDs 獲取設備對象。

clCreateContext -> clCreateCommandQueueWithProperties -> clCreateBuffer -> 

clEnqueueReadBuffer -> clCreateProgramWithSource -> clBuildProgram ->

clCreateKernel -> clSetKernelArg -> clEnqueueNDRangeKernel -> clEnqueueWriteBuffer

-> 釋放所有資源

```

OpenCL_API
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

### clCreateContext

```
cl_context clCreateContext(const cl_context_properties *properties,
                           cl_uint num_devices,
                           const cl_device_id *devices,
                           void ( CL_CALLBACK *pfn_notify)(const char *errinfo, const void *private_info, size_t cb, void *user_data),
                           void *user_data,
                           cl_int *errcode_ret)

選擇獲取的設備，創建上下文Context。使用的API是clCreateContext。OpenCL 使用Context 
管理命令隊列、程序內核、內存等資源對象。

參數說明：
- properties [IN] ：上下文屬性數組，每一項屬性包含一個枚舉常量，一個屬性值，數組以0 結尾。properties指定了創建Context 基於的Platform，也可以設為NULL，程序實現時自行選擇Platform。
- num_devices [IN] ：devices中指定的設備數。
- devices [IN] ：clGetDeviceIDs返回的設備對像數組。
- pfn_notify [IN] ：註冊回調函數，當OpenCL 創建上下文失敗時會執行回調函數。沒有回調可設為NULL
- user_data [INOUT]：傳遞給回調函數pfn_notify的指針參數，可設為NULL。
- errcode_ret [OUT] ：返回錯誤碼，如果errcode_ret設為NULL 不再返回錯誤碼。
- Return ：OpenCL 上下文成功創建時，返回創建的cl_contex t對象，errcode_ret返回CL_SUCCESS。創建失敗時返回NULL，errorcode_ret返回錯誤碼。

```

### clCreateCommandQueueWithProperties

```
cl_command_queue clCreateCommandQueueWithProperties(cl_context context,
                                                    cl_device_id device,
                                                    const cl_queue_properties *properties,
                                                    cl_int *errcode_ret)

為單個設備創建命令隊列，使用的API 是clCreateCommandQueueWithProperties。操作命令入隊後依據隊列屬性順序或者亂序執行。

參數說明：
- context [IN] ：有效的上下文對象cl_context。
- device [IN] ：與context關聯的設備。
- properties [IN] ：命令隊列屬性數組，每一項屬性包含一個枚舉常量，一個屬性值，數組以0 結尾。參考表2-4。
- errcode_ret [OUT]：返回錯誤碼，如果errcode_ret設為NULL 不再返回錯誤碼。
- Return ：OpenCL 命令隊列成功創建時，返回創建的 cl_command_queue 對象，errcode_ret返回CL_SUCCESS。創建失敗時返回NULL，errorcode_ret返回錯誤碼。

```

### clCreateBuffer

```
cl_mem clCreateBuffer(cl_context context,
                        cl_mem_flags flags,
                        size_t size,
                        void *host_ptr,
                        cl_int *errcode_ret)

創建Buffer 對象。

參數說明：
- context [IN] ：為context對象分配buffer 對象。
- flags [IN] ：以組合bit 位枚舉常量的方式，指定buffer 的分配和使用信息，參考表2-8。
- size [IN] ：申請內存空間字節數。
- host_ptr [IN] ：應用程序在Host 端已經申請的內存空間指針。
- errcode_ret [OUT]：返回錯誤碼。errcode_ret設為NULL 則不再返回錯誤碼。
- Return ：buffer 對象成功創建時，返回創建的 cl_mem 對象，errcode_ret返回CL_SUCCESS。創建失敗時返回NULL，errorcode_ret返回錯誤碼。

```

### clEnqueueRead/Write Buffer

```
cl_int clEnqueueReadBuffer (cl_command_queue command_queue,
                            cl_mem buffer,
                            cl_bool blocking_read,
                            size_t offset,
                            size_t size,
                            void *ptr,
                            cl_uint num_events_in_wait_list,
                            const cl_event *event_wait_list,
                            cl_event *event)

cl_int clEnqueueWriteBuffer(cl_command_queue command_queue,
                            cl_mem buffer,
                            cl_bool blocking_write,
                            size_t offset,
                            size_t size,
                            const void *ptr,
                            cl_uint num_events_in_wait_list,
                            const cl_event *event_wait_list,
                            cl_event *event)

clEnqueueReadBuffer從buffer 對象讀取數據到Host 端內存，clEnqueueWriteBuffer將Host 內存數據寫入buffer 對象。

參數說明：
- command_queue [IN] ：讀寫指令入隊的命令隊列對象。命令隊列和buffer 應在同一context下創建。
- buffer [IN] ：有效的buffer 對象。
- blocking_write和blocking_read [IN]：如果設置為CL_TRUE，表示阻塞式操作，數據傳輸結束才能返回。CL_FALSE表示非阻塞操作，命令入隊立即返回。
- offset [IN] ：讀寫buffer object 的字節偏移量。
- size [IN] ：讀寫數據的字節數。
- ptr [IN/OUT] ：host 端內存緩衝區地址。
- num_events_in_wait_list [IN] ：event_wait_list內事件的數目。
- event_wait_list [IN] ：cl_event數組。執行內核操作前，需要等待event_wait_list內事件執行完成。如果不需要等待，event_wait_list設為NULL，num_events_in_wait_list設為0。
- event [OUT] ：如果非NULL，則此參數用於查詢命令執行狀態，或等待命令完成。clEnqueue指令普遍包含num_events_in_wait_list，event_wait_list，event這三個參數。
- Return ：正常執行返回CL_SUCCESS

```


### clCreateProgramWithSource

```
cl_program clCreateProgramWithSource(cl_context context,
                                     cl_uint count,
                                     const char **strings,
                                     const size_t *lengths,
                                     cl_int *errcode_ret)

這一步通過OpenCL C 源碼字符串或程序二進制兩種方式之一創建內核程序Program。編譯Program 生成二進制
，檢查編譯錯誤並獲取二進制代碼。使用二進制代碼創建Program 能顯著減少編譯時間。

參數說明：
- context [IN] ：有效的Context對象。
- count [IN] ：表示strings中字符串的個數。
- strings [IN] ：字符串數組指針，所有的字符串構成設備源代碼。
- lengths [IN] ：表示strings每個字符串的長度。lengths可以設為NULL，字符串以0 結尾自動計算長度。
- errcode_ret [OUT]：返回錯誤碼。errcode_ret設為NULL 則不再返回錯誤碼。
- Return ：Program 對象成功創建時，返回創建的 cl_program 對象，errcode_ret返回CL_SUCCESS。創建失敗時返回NULL，errorcode_ret返回錯誤碼。

```

### clBuildProgram

```
cl_int clBuildProgram(cl_program program,
                      cl_uint num_devices,
                      const cl_device_id *device_list,
                      const char *options,
                      void (CL_CALLBACK *pfn_notify)( cl_program program, void *user_data),
                      void *user_data)

使用Program 源代碼或者二進制生成設備可執行程序。

參數說明：
- program [IN] ：創建的程序對象。
- num_devices [IN] ：device_list中的設備數目。
- device_list [IN] ：program關聯的設備對像數組。device_list設為NULL 時，為program關聯的所有設備編譯可執行程序。device_list非空則僅為device_list中給出的設備編譯可執行程序。
- options [IN] ：表示編譯選項的字符串，例如按OpenCL2.0 標準編譯，-cl-std=CL2.0。請參考[1]中5.8.4節CompilerOptions。
- pfn_notify [IN] ：註冊回調函數。如果pfn_notify非空，clBuildProgram在編譯開始後立即返回，生成結束時異步調用回調函數。如果pfn_notify為空，clBuildProgram必須等待生成結束才能返回。
- user_data [INOUT]：傳遞給回調函數的參數，可以設置為NULL。
- Return ：成功生成可執行程序返回CL_SUCCESS，錯誤碼參考[1]中5.8.2 節。

clBuildProgram包含編譯和鏈接過程，OpenCL2.0 也可以單獨調用 clCompileProgram 編譯，調用clLinkProgram 鏈接。

```

### clCreateKernel

```
cl_kernel clCreateKernel(cl_program program,
                         const char *kernel_name,
                         cl_int *errcode_ret)

根據內核函數名，從program對象創建kernel 對象。

參數說明：
- program [IN] ：已經生成可執行二進制的內核程序對象。
- kernel_name [IN] ：program中以 __kernel 修飾的函數名。
- errcode_ret [OUT]：返回錯誤碼，如果errcode_ret設為NULL 不再返回錯誤碼。
- Return ：Kernel對象成功創建時，返回創建的 cl_kernel 對象，errcode_ret返回CL_SUCCESS。創建失敗時返回NULL，errorcode_ret返回錯誤碼。

```

### clSetKernelArg

```
cl_int clSetKernelArg(cl_kernel kernel,
                      cl_uint arg_index,
                      size_t arg_size,
                      const void *arg_value)

準備好kernel 對象和輸入輸出mem 對像後，這一步設置kernel 函數參數，設置kernel函數的單個參數。

參數說明：
- kernel [IN] ：有效的內核對象。
- arg_index [IN]：內核函數參數索引，對於n 個參數的內核函數，arg_index從0 編號到n-1。
- arg_size [IN] ：第arg_index個參數佔內存大小。
- arg_value [IN]：指向傳入參數數據的指針。
- Return ：成功生成可執行程序返回CL_SUCCESS

```

### clEnqueueNDRangeKernel

```
cl_int clEnqueueNDRangeKernel(cl_command_queue command_queue,
                              cl_kernel kernel,
                              cl_uint work_dim,
                              const size_t *global_work_offset,
                              const size_t *global_work_size,
                              const size_t *local_work_size,
                              cl_uint num_events_in_wait_list,
                              const cl_event *event_wait_list,
                              cl_event *event)

設置kernel 的任務網格尺寸，並執行內核，提交內核執行命令，提交後API 立即返回Host。設備會按照設定的work-item 網格尺寸啟動內核函數執行。

參數說明：
- command_queue [IN] ：提交內核執行命令的命令隊列。
- kernel [IN] ：在設備上執行的內核函數。
- work_dim [IN] ：work-item 的組織維度，0 < work_dim <= 3，全局work-item 和工作組內work-item維度相同。
- global_work_offset [IN] ：數組，表示0~ work_dim -1 維全局工作項ID 的偏移量。可設為NULL，每個維度偏移量為0。
- global_work_size [IN] ：全局工作項尺寸數組，全局工作項總數為global_work_size[0]*...* global_work_size[work_dim-1]。
- local_work_size [IN] ：工作組尺寸數組，工作組內工作項個數為local_work_size[0]*...* local_work_size[work_dim-1]。
- num_events_in_wait_list [IN]：event_wait_list內事件的數目。
- event_wait_list [IN] ：cl_event數組。執行內核操作前，需要等待event_wait_list內事件執行完成。如果不需要等待，event_wait_list設為NULL，num_events_in_wait_list設為0。
- event [OUT] ：如果非NULL，則此參數用於查詢命令執行狀態，或等待命令完成。

```

### clReleaseMemObject

```
cl_int clReleaseMemObject(cl_mem memobj)

清理OpenCL 資源

參數說明：
- memobj [IN]：減少memobj的引用計數。
- Return ：正常執行返回CL_SUCCESS

```

### clReleaseKernel

```
cl_int clReleaseKernel (cl_kernel kernel)

清理OpenCL 資源

參數說明：
- kernel [IN]：減少kernel的引用計數。
- Return ：正常執行返回CL_SUCCESS

```

### clReleaseProgram

```
cl_int clReleaseProgram (cl_program program)

清理OpenCL 資源

參數說明：
- program [IN]：減少program的引用計數。
- Return ：正常執行返回CL_SUCCESS

```

### clReleaseCommandQueue

```
cl_int clReleaseCommandQueue (cl_command_queue command_queue)

清理OpenCL 資源

參數說明：
- command_queue [IN]：減少command_queue的引用計數。
- Return ：正常執行返回CL_SUCCESS

```

### clReleaseContext

```
cl_int clReleaseContext (cl_context context)

清理OpenCL 資源

參數說明：
- context [IN]：減少context的引用計數。
- Return ：正常執行返回CL_SUCCESS

```
