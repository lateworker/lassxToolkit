# lassxTookit

## Version 4.0.0 Beta

### **注意! RealESR_Tools 已正式更名为 lassxTookit! 早于 4.0.0 的版本暂时不受影响! 可自行去 Release 中下载**

Version 4.0.0 Beta 为内测版本, 有任何 '使用方面的问题 / BUG' 应在 '[B站](https://space.bilibili.com/454920362) / [Github](https://github.com/lateworker/lassxTookit) / QQ群 (935718273) / 洛谷' 提出. (当然, 你也可以选择憋着不说)

## 使用方法

**从此版本起, 使用时应将配置文件拖入主程序, 而不是直接打开主程序!**

也就是说, 它不再局限于单个配置文件.

### 配置文件介绍

配置文件为严格的 .json 文件. 任何格式错误都会导致, 程序在控制台输出 "ERROR" 并跳过此配置.

以下是配置文件的必填内容:

- "input_path": " /\*输入文件夹的路径\*/ "

- "output_path": " /\*输出文件夹的路径\*/ "

- "temp_path": " /\*临时文件夹的名称\*/"
  
  可以是多层目录, 不能出现 ".\\" 或 "..\\".

- "traverse_file": " /\*此文件用来存放运行数据\*/ "
  
  可以随意命名, 最好是 .json 格式. 此文件内存放了已处理文件的基本信息, 可供插件读取. (插件相关见后文)

- "selector": " /\*选择器, 用来匹配文件\*/ "
  
  在文件夹遍历时会用到选择器. 例子: \* (全部文件); \*.jpg (全部的 .jpg 格式文件).

- "model": " /\*超分选用的AI模型\*/ "
  
  详解见后文.

- "scale": " /\*模型的放大倍数\*/ "

- "denoise": " /\*模型的降噪等级\*/ "
  
  有些模型不支持选择降噪等级, 这时此项无任何意义, 但仍然是必填项.

- "syncgap": " /\*图片分块等级\*/ "
  
  我也不知道有什么效果, 但就是有模型用到了这个参数, 默认填 0 就好. 其余属性同 "denoise".

- "match" : \[ /\*正则表达式形式的黑名单\*/ \]
  
  可以筛选文件的名称 ("name") 和目录 ("path"), 现有完全匹配 ("match") , 部分匹配 ("search") 两种模式.

- "tree_restore" : true / false
  
  是否还原目录结构

- "subdir_find": true / false
  
  是否递归处理子目录

- "emptydir_find": true / false
  
  是否处理 不包含目标文件 的目录

- "addon": \[ /\*插件的名称\*/ \]
  
  自己写的插件要放到项目根目录下 execute 文件夹里. 默认会引用 "move_out.exe".
  
  "move_out.exe" 会在所有图片处理完成后 将所有图片移出临时文件夹.

### 模型介绍 (从之前的说明文档里拿来的)

- `realesrgan`

- `realesrnet`

- `realesrgan-anime`

- `realcugan`

- `waifu2x-anime`

- `waifu2x-photo`

- `DF2K`

- `DF2K-JPEG`

- AI 模型使用显卡放大图片, 处理速度取决于显卡算力和图片分辨率. 

- 模型之间有一定的差异, 具体可见`testimagine.7z`.

- `realesrgan-anime`模型适合超分动漫图片.

- `realesrnet`&`waifu2x-photo`模型适合超分真实图片.

- 若没有独显或独显很弱, 建议使用`realesrgan-anime`模型. 此模型处理速度最快.

- `realesrgan-anime`模型为Ram版本, 没有独显也可以超分大图片.

- 除`realesrgan-anime`以外所有模型都是非Ram版本, 显存和内存不足8G可能会导致崩溃, 通常在处理30MB以上或者10^8(一亿) 像素以上会崩溃.

- 显卡测试1: RX588 ARCAEA-8K-HKT.png 16MB 7680*4320 x4plus模型-用时30min Anime模型-用时14min. ~~这是Tptr的朋友提供的~~

- 注意: 如果使用独显, 在任务管理器中看不到显卡占用/NA软件中占用满/集显可以直接看到.

- **对真实图片进行超分不能使用anime模型 !**
