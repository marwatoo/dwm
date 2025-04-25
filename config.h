/* See LICENSE file for copyright and license details. */
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <X11/XF86keysym.h>
/* appearance */
static const char vic[]="Victoria-pc%:";		/* Dmenu name, Victoria*/
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Noto sans:style=bold:size=10","Noto Sans Arabic:size=10","JetBrains Mono Nerd Font:size=11","Font Awesome 6 Sharp Solid:pixelsize=11"," FontAwesome:pixelsize=11"};
static const char dmenufont[]       = "Noto sans:size=10";
static const char col_gray1[]       = "#282A36";
static const char col_gray2[]       = "#44475A";
static const char col_gray3[]       = "#BD93F9";
static const char col_gray4[]       = "#BD93F9";
static const char col_cyan[]        = "#6272A4";

// Nord Theme Colors for dwm

static const char col_nord0[]  = "#2E3440";  // Darkest background
static const char col_nord1[]  = "#3B4252";  // Slightly lighter background
static const char col_nord2[]  = "#434C5E";  // Unfocused window border
static const char col_nord3[]  = "#4C566A";  // Comments, secondary text

static const char col_nord4[]  = "#D8DEE9";  // Main text
static const char col_nord5[]  = "#E5E9F0";  // Lighter text
static const char col_nord6[]  = "#ECEFF4";  // Brightest text

static const char col_nord7[]  = "#8FBCBB";  // Cyan highlight
static const char col_nord8[]  = "#88C0D0";  // More cyan highlight
static const char col_nord9[]  = "#81A1C1";  // Blue highlight
static const char col_nord10[] = "#5E81AC";  // Darker blue

static const char col_nord11[] = "#BF616A";  // Red (error, urgent)
static const char col_nord12[] = "#D08770";  // Orange (warning)
static const char col_nord13[] = "#EBCB8B";  // Yellow (attention)
static const char col_nord14[] = "#A3BE8C";  // Green (success)
static const char col_nord15[] = "#B48EAD";  // Purple (special case)

// Dracula Theme Colors for dwm

static const char col_drac0[]  = "#282a36";  // Background
static const char col_drac1[]  = "#44475a";  // Current Line
static const char col_drac2[]  = "#6272a4";  // Selection
static const char col_drac3[]  = "#f8f8f2";  // Foreground (text)
static const char col_drac4[]  = "#8be9fd";  // Cyan
static const char col_drac5[]  = "#50fa7b";  // Green
static const char col_drac6[]  = "#ffb86c";  // Orange
static const char col_drac7[]  = "#ff79c6";  // Pink
static const char col_drac8[]  = "#bd93f9";  // Purple
static const char col_drac9[]  = "#ff5555";  // Red
static const char col_drac10[] = "#f1fa8c";  // Yellow

// Define color scheme for dwm
static const char *colors[][3] = {
    /*               fg          bg          border  */
    // [SchemeNorm] = { col_nord4,  col_nord0,  col_nord2 },  // Unfocused window
    // [SchemeSel]  = { col_nord6,  col_nord9,  col_nord8 },  // Focused window
    [SchemeNorm] = { col_drac3,  col_drac0,  col_drac1 },  // Unfocused window (Dracula Default)
    [SchemeSel]  = { col_drac3,  col_drac8,  col_drac7 },  // Focused window (Dracula Default)
};


/* tagging */
// static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
 static const char *tags[] = { "", "", "", "", "", "", "", "󰅱", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "firefox",     							NULL,       NULL,       1 << 0,       0,           -1 },
	{ "zen",     								NULL,       NULL,       1 << 0,       0,           -1 },
	{ "Vivaldi-stable",     					NULL,       NULL,       1 << 0,       0,           -1 },
	{ "Google-chrome",     						NULL,       NULL,       1 << 0,       0,           -1 },
	{ "Opera",     								NULL,       NULL,       1 << 0,       0,           -1 },
	{ "Brave-browser",     						NULL,       NULL,       1 << 0,       0,           -1 },
	{ "Xfce4-terminal",  						NULL,       NULL,       1 << 1,       0,           -1 },
	{ "XTerm",  								NULL,       NULL,       1 << 1,       0,           -1 },
	{ "Pcmanfm",  								NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Code",  									NULL,       NULL,       1 << 7,       0,           -1 },
	{ "kdevelop",  								NULL,       NULL,       1 << 7,       0,           -1 },
	{ "Gimp-3.0",  							    NULL,       NULL,       1 << 3,       0,           -1 },
	{ "Spotify",  								NULL,       NULL,       1 << 5,       0,           -1 },
	{ "FreeTube",  								NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Xed",  									NULL,       NULL,       1 << 7,       0,           -1 },
	{ "Pluma",  								NULL,       NULL,       1 << 7,       0,           -1 },
	{ "obsidian",  								NULL,       NULL,       1 << 4,       0,           -1 },
	{ "TelegramDesktop",    					NULL,       NULL,       1 << 6,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "󰬔",      monocle },
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

/* autostart */
static const char *const autostart[] = {
    "nitrogen --restore",
    "xrandr -s 1600x900",
    "picom --config ~/.config/picom/dwm.conf",
    "libinput-gestures-setup start",
    "setxkbmap -option grp:caps_toggle fr,ma",
    "lxpolkit",
    "/usr/lib/x86_64-linux-gnu/xfce4/notifyd/xfce4-notifyd",
    "volumeicon",
    "flameshot",
    "xfce4-clipman",
    "nm-tray",
    "xset b off",
    "fbxkb",
    "cbatticon",
	"kdeconnect-indicator",
    NULL /* terminate */
};

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-p", vic, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4,"-b", NULL };
static const char *termcmd[]  = { "xfce4-terminal", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,             			XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_Left,   shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_Right,  shiftview,      {.i = +1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_s,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_j,      aspectresize,   {.i = +24} },
	{ MODKEY|ShiftMask,             XK_k,      aspectresize,   {.i = -24} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	{ 0,                            XK_F1,      view,           {.i = 1 << 0} },
    { 0,                            XK_F2,      view,           {.i = 1 << 1} },
    { 0,                            XK_F3,      view,           {.i = 1 << 2} },
    { 0,                            XK_F4,      view,           {.i = 1 << 3} },
    { 0,                            XK_F5,      view,           {.i = 1 << 4} },
    { 0,                            XK_F6,      view,           {.i = 1 << 5} },
    { 0,                            XK_F7,      view,           {.i = 1 << 6} },
    { 0,                            XK_F8,      view,           {.i = 1 << 7} },
    { 0,                            XK_F9,      view,           {.i = 1 << 8} },
	{ MODKEY,             			XK_l,      spawn,          SHCMD("~/.config/rofi/powermenu/type-2/powermenu.sh") },
	{ MODKEY,             			XK_q,      spawn,          SHCMD("~/.config/rofi/launchers/type-1/launcher.sh") },
	{ MODKEY,             			XK_w,      spawn,          SHCMD("~/.config/rofi/applets/bin/browser.sh") },
	{ MODKEY,             			XK_e,      spawn,          SHCMD("pcmanfm ~") },
	{ MODKEY,             			XK_o,      spawn,          SHCMD("obsidian") },
	{ 0,			XF86XK_AudioLowerVolume,   spawn,			SHCMD("amixer -D pulse sset Master 10%-") },
	{ 0,			XF86XK_AudioRaiseVolume,   spawn, 			SHCMD("amixer -D pulse sset Master 10%+") },
	{ 0,			XF86XK_MonBrightnessUp,    spawn,			SHCMD("~/.config/openbox/scripts/brightness.sh 0.1") },
	{ 0,			XF86XK_MonBrightnessDown,  spawn,			SHCMD("~/.config/openbox/scripts/brightness.sh -0.1") },
	{ 0,			XF86XK_AudioMute,          spawn,			SHCMD("amixer -D pulse set Master 1+ toggle") },
	{ MODKEY, 						XK_F1,		tag, 			{.i = 1 << 0} },
	{ MODKEY, 						XK_F2,		tag, 			{.i = 1 << 1} },
	{ MODKEY, 						XK_F3,		tag, 			{.i = 1 << 2} },
	{ MODKEY, 						XK_F4,		tag, 			{.i = 1 << 3} },
	{ MODKEY, 						XK_F5, 		tag, 			{.i = 1 << 4} },
	{ MODKEY, 						XK_F6, 		tag, 			{.i = 1 << 5} },
	{ MODKEY, 						XK_F7, 		tag, 			{.i = 1 << 6} },
	{ MODKEY, 						XK_F8, 		tag, 			{.i = 1 << 7} },
	{ MODKEY, 						XK_F9, 		tag, 			{.i = 1 << 8} }

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

#endif // CONFIG_H_INCLUDED
