# Unit Step16 Output Component

## 功能说明

Unit Step16 的 output 组件允许你通过 ESPHome 的标准 output 接口控制：
- LED 显示屏亮度
- RGB LED 亮度
- RGB LED 的红、绿、蓝颜色通道

## 使用方法

### 1. 基础 Output 配置

```yaml
output:
  # LED 亮度输出 (0-100%)
  - platform: unit_step16
    id: led_brightness_output
    unit_step16_id: my_step16
    channel: led_brightness
  
  # RGB 亮度输出 (0-100%)
  - platform: unit_step16
    id: rgb_brightness_output
    unit_step16_id: my_step16
    channel: rgb_brightness
  
  # RGB 颜色通道 (0-255)
  - platform: unit_step16
    id: rgb_red_output
    unit_step16_id: my_step16
    channel: rgb_red
  
  - platform: unit_step16
    id: rgb_green_output
    unit_step16_id: my_step16
    channel: rgb_green
  
  - platform: unit_step16
    id: rgb_blue_output
    unit_step16_id: my_step16
    channel: rgb_blue
```

### 2. 配合 Light 组件使用

通过 output 组件，你可以将 Step16 的 LED 和 RGB 作为标准灯光实体使用：

```yaml
light:
  # RGB 彩色灯
  - platform: rgb
    name: "Step16 RGB Light"
    red: rgb_red_output
    green: rgb_green_output
    blue: rgb_blue_output
    effects:
      - pulse:
      - strobe:
      - random:
  
  # LED 显示屏亮度
  - platform: monochromatic
    name: "Step16 LED Display"
    output: led_brightness_output
```

### 3. 可用的通道类型

| Channel 值 | 说明 | 范围 | 用途 |
|-----------|------|------|------|
| `led_brightness` | LED 显示屏亮度 | 0-100% | 控制 7 段显示屏的亮度 |
| `rgb_brightness` | RGB LED 亮度 | 0-100% | 控制 RGB LED 的总体亮度 |
| `rgb_red` | RGB 红色通道 | 0-255 | 设置红色分量 |
| `rgb_green` | RGB 绿色通道 | 0-255 | 设置绿色分量 |
| `rgb_blue` | RGB 蓝色通道 | 0-255 | 设置蓝色分量 |

### 4. 在 Home Assistant 中使用

配置后，在 Home Assistant 中会显示为：
- 一个 RGB 灯光实体（可以选择颜色）
- 一个单色灯光实体（LED 显示屏亮度）
- 支持所有标准灯光控制功能（亮度、颜色、效果等）

### 5. 通过自动化控制

```yaml
# 示例：根据编码器位置改变颜色
sensor:
  - platform: unit_step16
    unit_step16_id: my_step16
    name: "Encoder Value"
    on_value:
      then:
        # 将 0-15 映射到色轮
        - light.turn_on:
            id: step16_rgb_light
            brightness: 100%
            red: !lambda |-
              int hue = x * 360 / 16;
              if (hue < 120) return (120 - hue) / 120.0;
              else if (hue < 240) return 0;
              else return (hue - 240) / 120.0;
            green: !lambda |-
              int hue = x * 360 / 16;
              if (hue < 120) return hue / 120.0;
              else if (hue < 240) return (240 - hue) / 120.0;
              else return 0;
            blue: !lambda |-
              int hue = x * 360 / 16;
              if (hue < 120) return 0;
              else if (hue < 240) return (hue - 120) / 120.0;
              else return (360 - hue) / 120.0;
```

### 6. 直接控制 Output

```yaml
# 使用按钮控制
button:
  - platform: template
    name: "Set LED Bright"
    on_press:
      - output.set_level:
          id: led_brightness_output
          level: 100%
  
  - platform: template
    name: "Set RGB Red"
    on_press:
      - output.set_level:
          id: rgb_red_output
          level: 100%
      - output.set_level:
          id: rgb_green_output
          level: 0%
      - output.set_level:
          id: rgb_blue_output
          level: 0%
```

## 文件结构

```
components/unit_step16/
├── __init__.py                    # 主组件配置
├── sensor.py                      # 传感器配置
├── unit_step16_sensor.h           # C++ 头文件
├── unit_step16_sensor.cpp         # C++ 实现
├── output/
│   ├── __init__.py               # Output 组件配置
│   └── unit_step16_output.h      # Output C++ 实现
├── example.yaml                   # 基础示例
├── example_with_lights.yaml       # 完整示例（含灯光）
└── OUTPUT_USAGE.md               # 本文档
```

## 优势

使用 output 组件的好处：
1. **标准接口**：使用 ESPHome 标准的 output 和 light 组件
2. **Home Assistant 集成**：自动在 HA 中显示为灯光实体
3. **丰富效果**：支持 ESPHome 的所有灯光效果（脉冲、闪烁、随机等）
4. **易于控制**：可以通过 HA 界面、自动化、脚本等多种方式控制
5. **状态同步**：状态会自动同步到 Home Assistant

## 完整示例

查看 `example_with_lights.yaml` 获取完整的配置示例。
