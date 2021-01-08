//-------------------------------------------------------------------------------------------------
//
//  File :  tim_var.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2020 Alain Royer.
// Email: aroyer.qc@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
// AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------------------------------
//
//  Notes : TIM variables for STM32F746NG
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef TIM_DRIVER_GLOBAL

const TIM_Info_t TIM_Info[NB_OF_TIM_DRIVER] =
{
  #if (TIM_DRIVER_SUPPORT_TIM1_CFG == DEF_ENABLED)
    {
        TIM1,                                   // TIMx
        RCC_APB2ENR_TIM1EN,                     // RCC_APBxENR
        &RCC->APB2ENR,                          // Register
        TIM1_UP_TIM10_IRQn,                     // IRQn_Channel
        GRBL_SPINDLE_USE_UPDATE_IRQ,            // IRQ enable for update?
        7,                                      // PreempPrio
        GRBL_SPINDLE_MODE,                      // Mode
        GRBL_SPINDLE_PRESCALER,                 // Prescaler
        GRBL_SPINDLE_RELOAD,                    // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM2_CFG == DEF_ENABLED)
    {
        TIM2,                                   // TIMx
        RCC_APB1ENR_TIM2EN,                     // RCC_APBxENR
        &RCC->APB1ENR,                          // Register
        TIM2_IRQn,                              // IRQn_Channel
        GRBL_LIMITS_DEBOUNCE_USE_UPDATE_IRQ,    // IRQ enable for update?
        7,                                      // PreempPrio
        GRBL_LIMITS_DEBOUNCE_MODE,              // Mode
        GRBL_LIMITS_DEBOUNCE_PRESCALER,         // Prescaler
        GRBL_LIMITS_DEBOUNCE_RELOAD,            // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM3_CFG == DEF_ENABLED)
    {
        TIM3,                                   // TIMx
        RCC_APB1ENR_TIM3EN,                     // RCC_APBxENR
        &RCC->APB1ENR,                          // Register
        TIM3_IRQn,                              // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM4_CFG == DEF_ENABLED)
    {
        TIM4,                                   // TIMx
        RCC_APB1ENR_TIM4EN,                     // RCC_APBxENR
        &RCC->APB1ENR,                          // Register
        TIM4_IRQn,                              // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM5_CFG == DEF_ENABLED)
    {
        TIM5,                                   // TIMx
        RCC_APB1ENR_TIM5EN,                     // RCC_APBxENR
        &RCC->APB1ENR,                          // Register
        TIM5_IRQn,                              // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM6_CFG == DEF_ENABLED)
    {
        TIM6,                                   // TIMx
        RCC_APB1ENR_TIM6EN,                     // RCC_APBxENR
        &RCC->APB1ENR,                          // Register
        TIM6_DAC_IRQn,                          // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM7_CFG == DEF_ENABLED)
    {
        TIM7,                                   // TIMx
        RCC_APB1ENR_TIM7EN,                     // RCC_APBxENR
        &RCC->APB1ENR,                          // Register
        TIM7_IRQn,                              // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM8_CFG == DEF_ENABLED)
    {
        TIM8,                                   // TIMx
        RCC_APB2ENR_TIM8EN,                     // RCC_APBxENR
        &RCC->APB2ENR,                          // Register
        TIM8_UP_TIM13_IRQn,                     // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM9_CFG == DEF_ENABLED)
    {
        TIM9,                                   // TIMx
        RCC_APB2ENR_TIM9EN,                     // RCC_APBxENR
        &RCC->APB2ENR,                          // Register
        TIM1_BRK_TIM9_IRQn,                     // IRQn_Channel
        GRBL_STEPPER_USE_UPDATE_IRQ,            // IRQ enable for update?
        7,                                      // PreempPrio
        GRBL_STEPPER_MODE,                      // Mode
        GRBL_STEPPER_PRESCALER,                 // Prescaler
        GRBL_STEPPER_RELOAD,                    // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM10_CFG == DEF_ENABLED)
    {
        TIM10,                                  // TIMx
        RCC_APB2ENR_TIM10EN,                    // RCC_APBxENR
        &RCC->APB2ENR,                          // Register
        TIM1_UP_TIM10_IRQn,                     // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM11_CFG == DEF_ENABLED)
    {
        TIM11,                                  // TIMx
        RCC_APB2ENR_TIM11EN,                    // RCC_APBxENR
        &RCC->APB2ENR,                          // Register
        TIM1_TRG_COM_TIM11_IRQn,                // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM12_CFG == DEF_ENABLED)
    {
        TIM12,                                  // TIMx
        RCC_APB1ENR_TIM12EN,                    // RCC_APBxENR
        &RCC->APB1ENR,                          // Register
        TIM8_BRK_TIM12_IRQn,                    // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM13_CFG == DEF_ENABLED)
    {
        TIM13,                                  // TIMx
        RCC_APB1ENR_TIM13EN,                    // RCC_APBxENR
        &RCC->APB2ENR,                          // Register
        TIM8_UP_TIM13_IRQn,                     // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_TIM14_CFG == DEF_ENABLED)
    {
        TIM8,                                   // TIMx
        RCC_APB1ENR_TIM14EN,                    // RCC_APBxENR
        &RCC->APB1ENR,                          // Register
        TIM8_TRG_COM_TIM14_IRQn,                // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif

  #if (TIM_DRIVER_SUPPORT_LPTIM_CFG == DEF_ENABLED)
    {
        LPTM1,                                  // TIMx
        RCC_APB1ENR_LPTIM1EN,                   // RCC_APBxENR
        &RCC->APB1ENR,                          // Register
        LPTIM1_IRQn,                            // IRQn_Channel
        true or false,                          // IRQ enable for update?
        7,                                      // PreempPrio
        TIM_OP_MODE_REPETITIVE,                 // Mode    or TIM_OP_MODE_SINGLE
        1,                                      // Prescaler
        1,                                      // Reload
    },
  #endif
};

class TIM_Driver myTIM_GRBL_Spindle         (GRBL_SPINDLE);
class TIM_Driver myTIM_GRBL_Stepper         (GRBL_STEPPER);

#ifdef ENABLE_SOFTWARE_DEBOUNCE
class TIM_Driver myTIM_GRBL_LimitsDebounce  (GRBL_LIMITS_DEBOUNCE);
#endif

#else // TIM_DRIVER_GLOBAL

extern const TIM_Info_t TIM_Info[NB_OF_TIM_DRIVER];

extern class TIM_Driver myTIM_GRBL_Spindle;
extern class TIM_Driver myTIM_GRBL_Stepper;
#ifdef ENABLE_SOFTWARE_DEBOUNCE
extern class TIM_Driver myTIM_GRBL_LimitsDebounce;
#endif

#endif // TIM_DRIVER_GLOBAL

//-------------------------------------------------------------------------------------------------
