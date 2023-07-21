/* See LICENSE file for copyright and license details. */
/* Constants */
#define TERMINAL "st"
#define BROWSER "firefox"
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 1;        /* horizontal padding for statusbar */
static const int vertpadbar         = 5;        /* vertical padding for statusbar */
static const char *fonts[]          = { "JetBrains Mono:style:Regular:size=10", "Symbols Nerd Font :size=11", "feather:size=11" };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "ranger", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {"discord", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",    spcmd1},
	{"ranger",    spcmd2},
	{"discord",   spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
    { "Gimp",         NULL,         NULL,       0,              1,           -1 },
    { "firefox",      NULL,         NULL,       1 << 1,         0,           -1 },
    { "mpv",          NULL,         NULL,       0,              1,           -1 },
    { "qBittorrent",  NULL,         NULL,       1 << 3,         0,           -1 },
    { "thunderbird",  NULL,         NULL,       1 << 2,         0,           -1 },
    { "Signal",       NULL,         NULL,       1 << 2,         0,           -1 },
    { "steam",        NULL,         NULL,       1 << 4,         1,           -1 },
    { "corectrl",     NULL,         NULL,       1 << 8,         1,           -1 },
    { "Virt-manager", NULL,         NULL,       1 << 7,         1,           -1 },
    { "Bitwarden",    NULL,         NULL,       0,              1,           -1 },
    { "floatterm",    NULL,         NULL,       0,              1,           -1 },
    { "Galculator",   NULL,         NULL,       0,              1,           -1 },
    { "Gcolor3",      NULL,         NULL,       0,              1,           -1 },
    { "Lxappearance", NULL,         NULL,       0,              1,           -1 },
    { "Nitrogen",     NULL,         NULL,       0,              1,           -1 },
    { "Nsxiv",        NULL,         NULL,       0,              1,           -1 },
    { "Pavucontrol",  NULL,         NULL,       0,              1,           -1 },
    { "qt5ct",        NULL,         NULL,       0,              1,           -1 },
    { "qt6ct",        NULL,         NULL,       0,              1,           -1 },
    { "kdenlive",     NULL,         NULL,       0,              1,           -1 },
    { "Nm-connection-editor",     NULL,         NULL,       0,              1,           -1 },
	{ NULL,       "spterm",     NULL,       SPTAG(0),       1,           -1 },
	{ NULL,       "ranger",     NULL,       SPTAG(1),       1,           -1 },
	{ NULL,       "discord",    NULL,       SPTAG(2),       1,           -1 },
    { "firefox", "Toolkit", "Picture-in-Picture", 0,        1,           -1},
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *termcmd[]  = { "st", NULL };
static const char *browser[] = { "firefox", NULL };
static const char *mail[] = {"thunderbird", NULL };
static const char *dmenucmd[] = { "dmenu_run", "-p", "Run", "-i", NULL };
static const char *rofi[]  = { "rofi_applauncher", NULL };

#include "movestack.c"
static const Key keys[] = {
    /* modifier                     key             function        argument */
    { MODKEY,                       XK_p,           spawn,       {.v = dmenucmd } },
    { MODKEY,                       XK_Return,      spawn,       {.v = termcmd } },
    { MODKEY,                       XK_d,           spawn,       {.v = rofi } },
    { MODKEY|ShiftMask,             XK_w,           spawn,       {.v = browser } },
    { MODKEY|ShiftMask,             XK_m,           spawn,       {.v = mail } },
    { MODKEY,                       XK_b,           togglebar,      {0} },
    { MODKEY,                       XK_j,           focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,           focusstack,     {.i = -1 } },
    { MODKEY,                       XK_Prior,       incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_Next,        incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,           setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,           setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_j,           movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,           movestack,      {.i = -1 } },
    { MODKEY,                       XK_plus,        incrgaps,       {.i = +5 } },
    { MODKEY,                       XK_minus,       incrgaps,       {.i = -5 } },
    { MODKEY|ShiftMask,             XK_plus,        incrigaps,      {.i = +5 } },
    { MODKEY|ShiftMask,             XK_minus,       incrigaps,      {.i = -5 } },
    { MODKEY|ControlMask,           XK_plus,        incrogaps,      {.i = +5 } },
    { MODKEY|ControlMask,           XK_minus,       incrogaps,      {.i = -5 } },
    { MODKEY,                       XK_g,           togglegaps,     {0} },
    { MODKEY|ShiftMask,             XK_g,           defaultgaps,    {0} },
    { MODKEY,                       XK_BackSpace,   zoom,           {0} },
    { MODKEY,                       XK_less,        view,           {0} },
    { MODKEY,                       XK_q,           killclient,     {0} },
    { MODKEY,                       XK_m,           setlayout,      {.v = &layouts[2]} },
    { MODKEY|ControlMask,           XK_comma,       cyclelayout,    {.i = -1 } },
    { MODKEY|ControlMask,           XK_period,      cyclelayout,    {.i = +1 } },
    { MODKEY,                       XK_space,       setlayout,      {0} },
    { MODKEY,                       XK_s,           togglesticky,   {0} },
    { MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },
    { MODKEY,                       XK_f,           togglefullscr,  {0} },
    { MODKEY,                       XK_0,           view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,           tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,       focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period,      focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,       tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period,      tagmon,         {.i = +1 } },
    { MODKEY,                       XK_t,           togglescratch,  {.ui = 0 } },
    { MODKEY,                       XK_r,           togglescratch,  {.ui = 1 } },
    { MODKEY,                       XK_c,           togglescratch,  {.ui = 2 } },
    { MODKEY,                       XK_F5,          xrdb,           {.v = NULL } },
    TAGKEYS(                        XK_1,                           0)
    TAGKEYS(                        XK_2,                           1)
    TAGKEYS(                        XK_3,                           2)
    TAGKEYS(                        XK_4,                           3)
    TAGKEYS(                        XK_5,                           4)
    TAGKEYS(                        XK_6,                           5)
    TAGKEYS(                        XK_7,                           6)
    TAGKEYS(                        XK_8,                           7)
    TAGKEYS(                        XK_9,                           8)
    { MODKEY|ControlMask,           XK_q,           quit,           {0} },
    { MODKEY|ControlMask,           XK_r,           quit,           {1} },
    /* XF86X */
    { 0,   XF86XK_AudioMute,        spawn,         SHCMD("status-volume mute") },
    { 0,   XF86XK_AudioLowerVolume, spawn,         SHCMD("status-volume down") },
    { 0,   XF86XK_AudioRaiseVolume, spawn,         SHCMD("status-volume up") },
    { 0,   XF86XK_Calculator,       spawn,          SHCMD("calculator" ) },
    /* Other bindings*/
    { MODKEY,                       XK_F1,          spawn,       SHCMD("pavucontrol") },
    { MODKEY,                       XK_F2,          spawn,       SHCMD("dmenu_network") },
    { MODKEY,                       XK_F3,          spawn,       SHCMD("thunar") },
    { MODKEY,                       XK_F4,          spawn,       SHCMD("rofi_kill_process") },
    { MODKEY,                       XK_F8,          spawn,       SHCMD("rofi_screenshot") },
    { MODKEY|ShiftMask,             XK_F8,          spawn,       SHCMD("rofi_screenshot -s") },
    { MODKEY,                       XK_F9,          spawn,       SHCMD("mic-toggle") },
    { MODKEY|ShiftMask,             XK_F9,          spawn,       SHCMD("dunst-toggle") },
    { MODKEY,                       XK_F11,         spawn,       SHCMD("betterlockscreen --lock blur") },
    { MODKEY|ShiftMask,             XK_F11,         spawn,       SHCMD("locker-toggler -t") },
    { MODKEY|ShiftMask,             XK_F12,         spawn,       SHCMD("picom-toggle") },
    { MODKEY,                       XK_section,     spawn,       SHCMD("dmenu_dots") },
    { MODKEY|ShiftMask,             XK_p,           spawn,       SHCMD("rofi_powermenu") },
    { MODKEY|ShiftMask,             XK_n,           spawn,       SHCMD(TERMINAL " -c floatterm -g 144x41  -e nvim ~/Documents/.vimwiki/notes.md") },
    { MODKEY,                       XK_Print,       spawn,       SHCMD("$HOME/.local/bin/quick-shot -f") },
    { MODKEY|ShiftMask,             XK_Print,       spawn,       SHCMD("$HOME/.local/bin/quick-shot -r") },
    { MODKEY|ControlMask,           XK_Print,       spawn,       SHCMD("$HOME/.local/bin/quick-shot -d") },
    { MODKEY|ControlMask,           XK_c,           spawn,       SHCMD("$HOME/.local/bin/dunst-calendar") },
    { MODKEY|ControlMask,           XK_i,           spawn,       SHCMD("$HOME/.local/bin/sysnfo") },
    { MODKEY|ControlMask,           XK_w,           spawn,       SHCMD("$HOME/.local/bin/weather-notify") },
    { MODKEY,                       XK_Tab,         spawn,       SHCMD("$HOME/.local/bin/rofiwindow") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
    { ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigstatusbar,   {.i = 6} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

