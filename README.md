# EmbarcaTech Projeto Final

Este projeto utiliza um microcontrolador Raspberry Pi Pico W para controlar a matriz de LEDs WS2818B e exibir mensagens em um display OLED SSD1306. O projeto permite interação com o usuário através dos botões A e B e o Joystick.

## Funcionalidades

- Controle de uma matriz de 25 LEDs WS2818B.
- Exibição de mensagens em um display OLED SSD1306.
- Leitura de entradas analógicas para controle de posição.
- Leitura de botões para mudança de cores.

## Esquema de Pinagem

| Componente       |  Pinos                    |
|------------------|---------------------------|
| LED WS2818B      | GPIO 7                    |
| Display OLED SDA | GPIO 14                   |
| Display OLED SCL | GPIO 15                   |
| Joystick (Eixo X)| GPIO 26 (ADC0)            |
| Joystick (Eixo Y)| GPIO 27 (ADC1)            |
| Botão A          | GPIO 5                    |
| Botão B          | GPIO 6                    |

## Referências

Este projeto foi influenciado por alguns projetos de exemplo do [BitDogLab-C](https://github.com/BitDogLab/BitDogLab-C), por exemplo :

- neopixel_pio
- Joystick_led
- display_oled

## Build

Esses comandos podem servir para a criaçao da pasta `build` e para a compilação do projeto.

```bash
cd EmbarcaTech_Final_Project
mkdir build
cd build
cmake -G "Ninja" ..
cmake --build .
```

## Video de Apresentação do Projeto

Para ver o video de apresentação do projeto, clique [Video de Apresentacao](https://www.youtube.com/)