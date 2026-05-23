```bash
docker build -t arm-gcc:latest -f Dockerfile.amd64 .
```

```bash
git clone https://github.com/Isla-jq/UAV.git
git submodule update
cd UAV/app
mkdir build&&cd build
cmake ..
make
```

# 指令

|     cmd      |   description   |
| :----------: | :-------------: |
|    help a    |    查看帮助     |
|   serlr a    |  设置液位基准   |
| saveconfig a | 保存配置到flash |

# 接口

|   Port    |     Interface     |
| :-------: | :---------------: |
|    PA6    | 接液位传感器的ADC |
| PA9、PA10 |       接NUC       |
|    USB    |  连电脑输入指令   |

# 注意

- `cmd`后面必须有参数，比如`help a`或者`help b`,什么都行，虽然用不到，但必须有
- 芯片内部`flash`有寿命限制，所以尽量少用`saveconfig a`

# 液位传感器解码

| 帧头 | 命令（液位） | 数值高字节  | 数值低字节 | 回车 | 换行 |
| :--: | :----------: | :---------: | :--------: | :--: | :--: |
| 0x55 |     0x01     | value[15:8] | value[7:0] |  \r  |  \n  |

注：解码后数据有符号，例：`int6`
