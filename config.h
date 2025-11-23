/* See LICENSE file for copyright and license details. */
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <X11/XF86keysym.h>
/* appearance */
static const char vic[]="Victoria%:";		/* Dmenu name, Victoria*/
static const unsigned int gappx     = 12;        /* gaps between windows */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMonoNL NFP:style=Bold:size=11", "Noto Sans Arabic:size=11:style=Bold","Noto Color Emoji:size=11" };
//static const char dmenufont[]       = "JetBrainsMonoNL NFP:size=11";
static const char col_gray1[]       = "#232629";  // Breeze background
static const char col_gray2[]       = "#31363B";  // Darker accent (unfocused border)
static const char col_gray3[]       = "#3DAEE9";  // Primary highlight (blue)
static const char col_gray4[]       = "#EFF0F1";  // Main text color
static const char col_cyan[]        = "#00BCD4";  // Cyan accent

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

// Core Breeze Dark palette
static const char col_breeze0[]  = "#232629";  // Background
static const char col_breeze1[]  = "#2A2E32";  // Alternate background
static const char col_breeze2[]  = "#31363B";  // Unfocused border / dark accent
static const char col_breeze3[]  = "#4D4D4D";  // Disabled text / dim gray

static const char col_breeze4[]  = "#EFF0F1";  // Main foreground (text)
static const char col_breeze5[]  = "#FCFCFC";  // Bright foreground (selected)
static const char col_breeze6[]  = "#00BCD4";  // Cyan accent
static const char col_breeze7[]  = "#3DAEE9";  // Blue highlight (KDE accent)

static const char col_breeze8[]  = "#DA4453";  // Red (error)
static const char col_breeze9[]  = "#F67400";  // Orange (warning)
static const char col_breeze10[] = "#C1D72E";  // Yellow/green (attention)
static const char col_breeze11[] = "#27AE60";  // Green (success)
static const char col_breeze12[] = "#9B59B6";  // Purple (special case)

// ================================
// dwm color scheme (Breeze Dark)
// ================================
// fg (text), bg, border
static const char *colors[][3] = {
    /*               fg            bg            border   */
    [SchemeNorm] = { col_breeze4,  col_breeze0,  col_breeze2 },  // Unfocused window
    [SchemeSel]  = { col_breeze5,  col_breeze7,  col_breeze7 },  // Focused window
};

/* tagging */
static const char *tags[] = { "󰾔", "󰐤", "󰉌", "󰇟", "󰏬","󰎄", "󰍦", "󱃖", "" };

/* Rules */

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
	{ "st",               						NULL,       NULL,       1 << 1,       0,           -1 },
	{ "XTerm",  								NULL,       NULL,       1 << 1,       0,           -1 },
	{ "Pcmanfm",  								NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Thunar",  								NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Code",  									NULL,       NULL,       1 << 7,       0,           -1 },
	{ "kdevelop",  								NULL,       NULL,       1 << 7,       0,           -1 },
	{ "Gimp-3.0",  							    NULL,       NULL,       1 << 3,       0,           -1 },
	{ "Gimp",  							    	NULL,       NULL,       1 << 3,       0,           -1 },
	{ "Spotify",  								NULL,       NULL,       1 << 5,       0,           -1 },
	{ "FreeTube",  								NULL,       NULL,       1 << 8,       0,           -1 },
	{ "io.github.celluloid_player.Celluloid",  								NULL,       NULL,       1 << 8,       0,           -1 },
	{ "mpv",  					    			NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Xed",  									NULL,       NULL,       1 << 7,       0,           -1 },
	{ "Pluma",  								NULL,       NULL,       1 << 7,       0,           -1 },
	{ "obsidian",  								NULL,       NULL,       1 << 4,       0,           -1 },
	{ "TelegramDesktop",    					NULL,       NULL,       1 << 6,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
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
    "/home/marwa/.config/dwm/setgamma.sh",
    "/home/marwa/.config/dwm/gamma.sh",
    "picom --config ~/.config/picom/dwm.conf",
    "libinput-gestures-setup start",
    "setxkbmap -option grp:caps_toggle fr,ma",
    "lxpolkit",
    "/usr/lib/x86_64-linux-gnu/xfce4/notifyd/xfce4-notifyd",
    "flameshot",
    "xfce4-clipman",
    "nm-tray",
    "xset b off",
    "megasync",
	"dwmblocks",
	"xfce4-screensaver",
	"xfce4-power-manager",
	"kdeconnect-indicator",
	"volumeicon",
    NULL /* terminate */
};

/* commands */
//static const char *dmenucmd[] = { "dmenu_run", "-p", vic, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4,"-b", NULL };
static const char *termcmd[]  = { "xfce4-terminal", NULL };

static const Key keys[] = {
	/* modifier                     key        function        	argument */
	{ MODKEY,             			XK_Return, spawn,          	{.v = termcmd } },
	{ MODKEY, 			            XK_r,      togglebar,      	{0} },
	{ MODKEY,                       XK_j,      focusstack,     	{.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     	{.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     	{.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     	{.i = -1 } },
	{ MODKEY,                       XK_Left,   shiftview,      	{.i = -1 } },
	{ MODKEY,                       XK_Right,  shiftview,      	{.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Right,  shifttag,        { .i = +1 } },
    { MODKEY|ShiftMask,             XK_Left,   shifttag,        { .i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       	{.f = -0.05} },
	{ MODKEY,                       XK_s,      setmfact,       	{.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           	{0} },
	{ MODKEY,                       XK_Tab,    view,           	{0} },
	{ MODKEY|ShiftMask,             XK_j,      aspectresize,   	{.i = +24} },
	{ MODKEY|ShiftMask,             XK_k,      aspectresize,   	{.i = -24} },
	{ MODKEY|Mod1Mask,              XK_c,      killclient,     	{0} },
	{ MODKEY,                       XK_t,      setlayout,      	{.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      	{.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      	{.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      	{0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, 	{0} },
	{ 0,                            XK_F1,     view,          	{.i = 1 << 0} },
    { 0,                            XK_F2,     view,          	{.i = 1 << 1} },
    { 0,                            XK_F3,     view,          	{.i = 1 << 2} },
    { 0,                            XK_F4,     view,          	{.i = 1 << 3} },
    { 0,                            XK_F5,     view,          	{.i = 1 << 4} },
    { 0,                            XK_F6,     view,          	{.i = 1 << 5} },
    { 0,                            XK_F7,     view,          	{.i = 1 << 6} },
    { 0,                            XK_F8,     view,          	{.i = 1 << 7} },
    { 0,                            XK_F9,     view,          	{.i = 1 << 8} },
    { MODKEY|ShiftMask,             XK_1,      tag,             {.ui = 1 << 0} },
    { MODKEY|ShiftMask,             XK_2,      tag,             {.ui = 1 << 1} },
    { MODKEY|ShiftMask,             XK_3,      tag,             {.ui = 1 << 2} },
    { MODKEY|ShiftMask,             XK_4,      tag,             {.ui = 1 << 3} },
    { MODKEY|ShiftMask,             XK_5,      tag,             {.ui = 1 << 4} },
    { MODKEY|ShiftMask,             XK_6,      tag,             {.ui = 1 << 5} },
    { MODKEY|ShiftMask,             XK_7,      tag,             {.ui = 1 << 6} },
    { MODKEY|ShiftMask,             XK_8,      tag,             {.ui = 1 << 7} },
    { MODKEY|ShiftMask,             XK_9,      tag,             {.ui = 1 << 8} },
    { MODKEY|ShiftMask,             XK_r,      spawn,          	SHCMD("~/.config/dwm/gamma.sh") },
	{ MODKEY,             			XK_l,      spawn,          	SHCMD("~/.config/rofi/powermenu/type-2/powermenu.sh") },
	{ MODKEY,             			XK_q,      spawn,          	SHCMD("~/.config/rofi/launchers/type-1/launcher.sh") },
	{ MODKEY,             			XK_w,      spawn,          	SHCMD("firefox") },
	{ MODKEY|ShiftMask,       		XK_w,      spawn,          	SHCMD("~/.config/rofi/applets/bin/browser.sh") },
	{ MODKEY,             			XK_e,      spawn,          	SHCMD("thunar ~") },
	{ MODKEY,             			XK_b,      spawn,          	SHCMD("obsidian") },
	{ MODKEY,             			XK_p,      spawn,          	SHCMD("flameshot gui") },
	{ MODKEY,             			XK_a,      spawn,          	SHCMD("~/.config/bspwm/pkde.py") },
	{ MODKEY,             			XK_z,      spawn,          	SHCMD("~/.config/bspwm/pemoji/pemoji.py") },
	{ MODKEY,             			XK_o,      spawn,          	SHCMD("~/.config/bspwm/blue.py") },
	{ MODKEY,             			XK_v,      spawn,          	SHCMD("~/.config/bspwm/yt.sh") },
	{ MODKEY,             			XK_x,      spawn,          	SHCMD("xed") },
	{ MODKEY,             			XK_u,      spawn,          	SHCMD("code --password-store=kwallet6") },
//	{ 0,			XF86XK_AudioLowerVolume,   spawn,			SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
//	{ 0,			XF86XK_AudioRaiseVolume,   spawn, 			SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
	{ 0,			XF86XK_MonBrightnessUp,    spawn,			SHCMD("~/.config/openbox/scripts/brightness.sh 0.1") },
	{ 0,			XF86XK_MonBrightnessDown,  spawn,			SHCMD("~/.config/openbox/scripts/brightness.sh -0.1") },
//	{ 0,			XF86XK_AudioMute,          spawn,			SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },
	{ 0,			XF86XK_AudioMicMute,       spawn,			SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ MODKEY, 						XK_F1,	   tag, 			{.i = 1 << 0} },
	{ MODKEY, 						XK_F2,	   tag, 			{.i = 1 << 1} },
	{ MODKEY, 						XK_F3,	   tag, 			{.i = 1 << 2} },
	{ MODKEY, 						XK_F4,	   tag, 			{.i = 1 << 3} },
	{ MODKEY, 						XK_F5, 	   tag, 			{.i = 1 << 4} },
	{ MODKEY, 						XK_F6, 	   tag, 			{.i = 1 << 5} },
	{ MODKEY, 						XK_F7, 	   tag, 			{.i = 1 << 6} },
	{ MODKEY, 						XK_F8, 	   tag, 			{.i = 1 << 7} },
	{ MODKEY, 						XK_F9, 	   tag, 			{.i = 1 << 8} },
//	TAGKEYS(                        XK_1,                                  0)
//	TAGKEYS(                        XK_2,                                  1)
//	TAGKEYS(                        XK_3,                                  2)
//	TAGKEYS(                        XK_4,                                  3)
//	TAGKEYS(                        XK_5,                                  4)
//	TAGKEYS(                        XK_6,                                  5)
//	TAGKEYS(                        XK_7,                                  6)
//	TAGKEYS(                        XK_8,                                  7)
//	TAGKEYS(                        XK_9,                                  8)

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
