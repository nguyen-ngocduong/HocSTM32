#include "main.h"
#include <string.h>
#include <stdio.h>

/* Bien cuc bo ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

uint16_t led_pins[] = {
    GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
    GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7
};
uint8_t current_led = 0;
int direction = 1; // 1: tien, -1: lui
uint8_t knightRiderRunning = 1; // Bien dieu khien hieu ung Knight Rider (1: chay, 0: dung)
uint16_t delay_time = 100; // Thoi gian delay ban dau (100 ms)

/* Dinh nghia ham */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void KnightRiderEffect(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void UART_Transmit(const char* message);
void UART_Receive(void); // Ham nhan du lieu tu UART

/* Ham chinh */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    UART_Transmit("Knight Rider Effect Started! Enter '+' to increase delay or '-' to decrease delay.\r\n");

    while (1)
    {
        if (knightRiderRunning) {
            KnightRiderEffect();
        }

        // Kiem tra du lieu UART
        UART_Receive();
    }
}

/* Hieu ung Knight Rider */
void KnightRiderEffect(void) {
    // Tat tat ca cac LED
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(GPIOA, led_pins[i], GPIO_PIN_RESET);
    }

    // Bat LED hien tai
    HAL_GPIO_WritePin(GPIOA, led_pins[current_led], GPIO_PIN_SET);

    // Cap nhat chi so LED hien tai
    current_led += direction;

    // Dao chieu khi den bien
    if (current_led == 7) {
        direction = -1;
    } else if (current_led == 0) {
        direction = 1;
    }

    HAL_Delay(delay_time);
}

/* Khoi tao GPIO */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Kich hoat clock cho GPIOA va GPIOB */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Cau hinh cac chan GPIO: PA0 den PA7 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                          GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Cau hinh PB0 lam nut bam */
    GPIO_InitStruct.Pin = GPIO_PIN_0; // PB0 cho nut bam
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;  // Kich hoat ngat khi co suon len
    GPIO_InitStruct.Pull = GPIO_PULLUP;          // Su dung dien tro keo len
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Khoi tao ngat EXTI cho PB0 */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/* Khoi tao USART1 */
static void MX_USART1_UART_Init(void) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
		huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

/* Ham callback EXTI khi nut bam (PB0) duoc nhan */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    static uint32_t last_interrupt_time = 0;
    uint32_t interrupt_time = HAL_GetTick();

    if (GPIO_Pin == GPIO_PIN_0) {
        // Chong doi nut (debounce)
        if (interrupt_time - last_interrupt_time > 300) {
            // Chuyen doi trang thai hieu ung LED
            knightRiderRunning = !knightRiderRunning;

            if (knightRiderRunning) {
                UART_Transmit("Knight Rider Effect Resumed.\r\n");
            } else {
                UART_Transmit("Knight Rider Effect Stopped.\r\n");

                // Tat tat ca LED khi hieu ung dung
                for (int i = 0; i < 8; i++) {
                    HAL_GPIO_WritePin(GPIOA, led_pins[i], GPIO_PIN_RESET);
                }
            }
        }
        last_interrupt_time = interrupt_time;
    }
}

/* Ham truyen UART */
void UART_Transmit(const char* message) {
    HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}

/* Ham nhan UART */
void UART_Receive(void) {
    uint8_t received_data;
    if (HAL_UART_Receive(&huart1, &received_data, 1, 10) == HAL_OK) {
        if (received_data == '+') {
            if (delay_time < 1000) {
                delay_time += 50;
                char buffer[50];
                snprintf(buffer, sizeof(buffer), "Speed Decreased to %d ms.\r\n", delay_time);
                UART_Transmit(buffer);
            }
        } else if (received_data == '-') {
            if (delay_time > 10) {
                delay_time -= 50;
                char buffer[50];
                snprintf(buffer, sizeof(buffer), "Speed Increased to %d ms.\r\n", delay_time);
                UART_Transmit(buffer);
            }
        }
    }
}

/* Cau hinh clock he thong */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {Error_Handler();
    }
}

/* Ham xu ly loi */
void Error_Handler(void) {
    __disable_irq();
    while (1)
    {
    }
}