RealESR Tools
Version 2.7.5 Release
特点:
	1.可以批量放大图片
	2.可以指定任意文件夹
	3.可以自动创建文件夹
	4.可以还原图片放大前的文件夹结构
使用方法:
	主程序: RealESR_Tools.exe
	界面:
		Version: 2.7.5 Release					// 版本号
		Process									// 开始处理图片
		Exit									// 退出程序
		
		Settings								// 设置板块
		- RealESR Mode:	[Your RealSR Mode]		// 设置RealSR模型
		- Input Path:	[Your Input Path]		// 设置输入文件或文件夹路径
		- Output Path:	[Your Output Path]		// 设置输出文件夹路径
		- Selector:		[Your Selector]			// 设置筛选器
		Save									// 保存设置
		Reset									// 将设置恢复为默认值
	设置板块详解:
		1.设置RealESR模型:
			RealSR模型有3种:
				1.realesrgan-x4plus
				2.realesrgan-x4plus-anime
				3.realesrnet-x4plus
			区别:
				真实性上，realesrgan-x4plus > realesrnet-x4plus > realesrgan-x4plus-anime
				速度上，realesrgan-x4plus-anime > realesrnet-x4plus > realesrgan-x4plus
		2.设置输入文件或文件夹路径:
			输入文件夹路径就是待处理图片的存放路径. !!输入文件夹必须已经存在!!
			程序会读取空格, 并会自动忽略:输入文本开头的双引号 以及 输入文本末尾的双引号和右斜杠. !!请勿使用左斜杠表示路径!!
			例如:	.\input
					.\input\"
					"C:\
					".\input"
			如果输入文本是一个文件路径, 则程序会单独处理此文件而不是整个文件夹.
			例如:	"C:\a.jpg"
			此外, 程序会将此类输入拆成:文件夹部分(C:\)和文件名部分(a.jpg), 程序将不会在[Your Input Path]一栏中显示完整文件名, 请知悉.
			// 上一行为举例说明,并不是一定会变成 "C:\" 和 "a.jpg"
		3.设置输出文件夹路径:
			输出文件夹路径就是存放处理完成的图片的文件夹路径. !!输出文件夹可以不存在!!
			程序会读取空格, 并会自动忽略:输入文本开头的双引号 以及 输入文本末尾的双引号和右斜杠. !!请勿使用左斜杠表示路径!!
			例如:	.\output
					"C:\
		4.设置筛选器:
			若一个文件夹内含有多种类型文件, 如: jpg, txt, png等, 而只有jpg格式图片需要处理, 就可以利用筛选器实现只处理jpg格式图片.
			暂时不支持同时筛选多种格式处理, 但可以全部处理(不筛选)
			只需要输入文件格式即可. !!请勿填入多余的任何符号!!
			例如: 	*	// 填*代表不筛选, 处理全部文件
					jpg
					png
					webp
		5.默认值:
			- RealESR Mode:	realsrgan-x4plus
			- Input Path:	.\input
			- Output Path:	.\output
			- Selector:		*
Tips:
	1.realESR使用显卡放大图片, 处理速度取决于显卡算力. 若没有独显或独显很弱, 建议使用realesr-x4plus-anime模型.
	2.如果不想手动输入文件夹路径, 可以右键文件或文件夹, 点击 "复制文件地址(A)" 按钮, 直接粘贴进程序输入框. 或者可以直接将文件或文件夹拖入程序输入框.
	3.与程序同级的"input"和"output"文件夹为示例文件夹, 若图片存放于其它文件夹, 就可以将其删除.
	4.Tptr在程序里埋藏了7个彩蛋, 若全部找出可以B站@Tptr-lateworker交作业哦.