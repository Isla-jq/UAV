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
