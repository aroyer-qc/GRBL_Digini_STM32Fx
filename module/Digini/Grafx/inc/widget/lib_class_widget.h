//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_widget.h
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

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "widget_cfg.h"

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class CWidgetInterface
{
    public:

        virtual            ~CWidgetInterface            (){};

        virtual Link_e      Create                      (PageWidget_t* pPageWidget)     = 0;
        virtual Link_e      Refresh                     (MsgRefresh_t* pMsg)            = 0;
        virtual void        Finalize                    (void)                          = 0;
};

#ifdef BACK_DEF
class CBackground : public CWidgetInterface
{
    public:
                            CBackground                 (Background_t* pBackground);
                           ~CBackground                 (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);

        Background_t*       m_pBackground;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef BASIC_BOX_DEF
class CBasicBox : public CWidgetInterface
{
    public:
                            CBasicBox                   (BasicBox_t* pBasicBox);
                           ~CBasicBox                   (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);
        void                DrawOnce                    (ServiceReturn_t* pService);

        BasicBox_t*         m_pBasicBox;
        Cartesian_t         m_TopLeft;
        Cartesian_t         m_TopRight;
        Cartesian_t         m_BottomLeft;
        Cartesian_t         m_BottomRight;

        // Horizontal bar info for draw
        int16_t             m_X_Hor;
        int16_t             m_Y_HorTop;
        int16_t             m_Y_HorBot;
        int16_t             m_HorUpTo;

        // Vertical bar info for draw
        int16_t             m_Y_Ver;
        int16_t             m_X_VerLeft;
        int16_t             m_X_VerRight;
        int16_t             m_VerUpTo;

        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef BASIC_BTN_DEF
class CBasicButton : public CWidgetInterface
{
    public:
                            CBasicButton                (BasicButton_t* pBasicButton);
                           ~CBasicButton                (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);
        void                DrawOnce                    (ServiceReturn_t* pService);

        BasicButton_t*      m_pBasicButton;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef BASIC_RECT_DEF
class CBasicRect : public CWidgetInterface
{
    public:
                            CBasicRect                   (BasicRect_t* pBasicRect);
                           ~CBasicRect                   (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);

        BasicRect_t*        m_pBasicRect;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef BTN_DEF
class CButton : public CWidgetInterface
{
    public:
                            CButton                     (Button_t* pButton);
                           ~CButton                     (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);
        void                DrawOnce                    (ServiceReturn_t* pService);

        Button_t*           m_pButton;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef GIF_DEF
class CGif : public CWidgetInterface
{
    public:
                            CGif                        (Gif_t* pGif);
                           ~CGif                        (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);
        void                DrawOnce                    (ServiceReturn_t* pService);

        Gif_t*              m_pGif;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef ICON_DEF
class CIcon : public CWidgetInterface
{
    public:
                            CIcon                       (Icon_t* pIcon);
                           ~CIcon                       (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);
        void                DrawOnce                    (ServiceReturn_t* pService);

        Icon_t*             m_pIcon;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef LABEL_DEF
class CLabel : public CWidgetInterface
{
    public:
                            CLabel                      (Label_t* pLabel);
                           ~CLabel                      (){};


        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);

        Label_t*            m_pLabel;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef LABEL_LIST_DEF
class CLabelList : public CWidgetInterface
{
    public:
                            CLabelList                  (LabelList_t* pLabelList);
                           ~CLabelList                  (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);

        LabelList_t*        m_pLabelList;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef METER_DEF
class CMeter : public CWidgetInterface
{
    public:
                            CMeter                      (Meter_t* pMeter);
                           ~CMeter                      (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);
        void                DrawOnce                    (ServiceReturn_t* pService);

        Meter_t*            m_pMeter;
        ServiceEvent_e      m_ServiceState;
        uint16_t            m_Value;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef PAGE_SLIDE_DEF
class CPageSlide : public CWidgetInterface
{
    public:
                            CPageSlide                  (PageSlide_t* pPageSlide);
                           ~CPageSlide                  (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);
        void                DrawOnce                    (ServiceReturn_t* pService);

        PageSlide_t*        m_pPageSlide;

        ServiceEvent_e      m_ServiceState;
        ServiceEvent_e      m_LastServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef PANEL_DEF
class CPanel : public CWidgetInterface
{
    public:
                            CPanel                      (Panel_t* pPanel);
                           ~CPanel                      (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);
        void                DrawOnce                    (ServiceReturn_t* pService);

        Panel_t*            m_pPanel;

        ServiceEvent_e      m_ServiceState;
        ServiceEvent_e      m_LastServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef PROGRESS_DEF
class CProgress : public CWidgetInterface
{
    public:
                            CProgress                   (Progress_t* pProgress);
                           ~CProgress                   (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);
        void                DrawOnce                    (ServiceReturn_t* pService);

        Progress_t*         m_pProgress;
        uint16_t            m_Range;
        ServiceEvent_e      m_ServiceState;
        uint16_t            m_Value;
        bool                m_NegativeMovement;
        PageWidget_t*       m_pPageWidget;
        Cartesian_t         m_CursorPos;
};
#endif

#ifdef SPECTRUM_DEF
class CSpectrum : public CWidgetInterface
{
    public:
                            CSpectrum                   (Spectrum_t* pSpectrum);
                           ~CSpectrum                   (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);

        Spectrum_t*         m_pSpectrum;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
        uint16_t            m_DotSize;
        BoxSize_t           m_BarSize;
        void*               m_BarAddress;
        PixelFormat_e       m_PixelFormat;
};
#endif

#ifdef SPRITE_DEF
class CSprite : public CWidgetInterface
{
    public:
                            CSprite                     (Sprite_t* pSprite);
                           ~CSprite                     (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (void);

        Sprite_t*           m_pSprite;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef TERMINAL_DEF
class CTerminal : public CWidgetInterface
{
    public:
                            CTerminal                   (Terminal_t* pTerminal);
                           ~CTerminal                   (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (ServiceReturn_t* pService);

        Terminal_t*         m_pTerminal;
        uint8_t             m_NumberOfLine;
        uint8_t             m_NbOfCharPerLine;
        uint8_t             m_OffsetLine;
        uint8_t             m_CurrentLine;
        uint8_t             m_LineSize;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
        uint8_t*            m_pScreen;
};
#endif

#ifdef VIRTUAL_HUB_DEF
class CVirtualHub : public CWidgetInterface
{
    public:
                            CVirtualHub                 (VirtualHub_t* pVirtualHub);
                           ~CVirtualHub                 (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        VirtualHub_t*       m_pVirtualHub;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

#ifdef VIRTUAL_WINDOW_DEF
class CVirtualWindow : public CWidgetInterface
{
    public:
                            CVirtualWindow              (VirtualWindow_t* pVirtualWindow);
                           ~CVirtualWindow              (){};

        Link_e              Create                      (PageWidget_t* pPageWidget);
        Link_e              Refresh                     (MsgRefresh_t* pMsg);
        void                Finalize                    (void);

    private:

        void                Draw                        (void);
        void                DrawOnce                    (ServiceReturn_t* pService);

        VirtualWindow_t*    m_pVirtualWindow;
        PixelFormat_e       m_PixelFormat;
        uint8_t*            m_pAddress;
        ServiceEvent_e      m_ServiceState;
        PageWidget_t*       m_pPageWidget;
};
#endif

//-------------------------------------------------------------------------------------------------

#endif //DIGINI_USE_GRAFX

