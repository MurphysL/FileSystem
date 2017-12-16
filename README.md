# FileSystem

# 文件系统


以 **Linux ext2** 为原型 ，模拟磁盘大小 **10M**，单分区

## 物理结构

### 扇区（section）

* **大小**：512 bytes
* **总数**： 20480

### 磁盘块（block）
* **大小**： 1024 bytes
* **总数**：10240

## 存储布局

### 用户块（user block)

用户信息

* **大小**：1024 bytes
* **结构体大小**：40 bytes
* **总数**：25
* **属性**：
	* **name**: 用户名（unique）
	* **psw**: 密码
	* **flag**: 删除标志
	* **su**: 用户权限
	* **next**

### 超级块（super block）

描述整个分区的文件系统信息

* **大小**：512 bytes
* **结构体大小**：56 bytes
* **总数**：9（8 个备份）
* **属性**：
	* **block_size**: 磁盘块大小 
	* **ver**: 文件系统版本号
	* **inode_size**: inode 区大小
	* **data_size**： data_block 区大小
	* **inode_begin**: inode 区始址
	* **data_begin**: data 区始址
	* **inode_num**: inode 数量
	* **data_num**: data block 数量
	* **remain_inode_num**: 剩余 inode 数量
	* **remain_data_num**：剩余 data block 数量
	* **mount_time**: 挂载时间
	* **last_write_time**: 最近写入数据时间
	* **mount**: 文件系统是否挂载
	* **valid**: 校验位

### inode 位图（inode bitmap）

描述 inode block 中 block 使用信息

* **大小**：1024 bytes
* **总数**：1024

### 块位图（data bitmap）

描述 data block 中 block 使用信息

* **大小**：10 kb
* **总数**：10240

### inode 表（inode table）

* **大小**：50kb
* **结构体大小**：52 bytes
* **总数**：984
* **属性**：
	* **no** : inode 编号
	* **size**: 文件字节数
	* **flag**: 文件/目录（1->文件，0->目录）
	* **data_num**: 所占 data block 数量
	* **data_begin**：首 data block 位置
	* **owner**: 拥有者
	* **power**: 权限
	* **create_time**: 创建时间
	* **update_time**: 修改时间
	* **read_time**: 访问时间
	* **delete**: 删除标志

### data 表 （data table）

* **可用大小**: 约 9.9 M
* **总数**：9578
* **结构体大小**：1088 bytes

#### 文件数据表

* **属性**：
	* **no** : data block 编号
	* **flag**: 文件数据/目录信息
	* **buffer**: 文件数据（1024 bytes）
	* **next**: 后继 data block
	* **delete**: 删除标志

#### 目录信息表

* **属性**：
	* **no** : data block 编号
	* **flag**: 文件数据/目录信息
	* **dir_name**：目录名 
	* **file_info**: 文件信息
	* **delete**: 删除标志


### 奇偶校验


**参考：**
http://www.i3geek.com/archives/1275
http://blog.csdn.net/belen_xue/article/details/51586178
