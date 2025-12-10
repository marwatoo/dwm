/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* key definitions */
#define MODKEY Mod4Mask

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

//TYPE							NAME					VALUE {CONTENT}
/* appearance */
static const unsigned int 		borderpx  				= 4;        /* border pixel of windows */
static const unsigned int 		gappx     				= 12;        /* gaps between windows */
static const unsigned int 		snap      				= 32;       /* snap pixel */
static const unsigned int 		systraypinning 			= 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int 		systrayonleft 			= 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int 		systrayspacing 			= 2;   /* systray spacing */
static const int 				systraypinningfailfirst	= 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int 				showsystray        		= 1;        /* 0 means no systray */
static const int 				showbar            		= 1;        /* 0 means no bar */
static const int 				topbar             		= 1;        /* 0 means bottom bar */
static const int 				vertpad            		= 10;       /* vertical padding of bar */
static const int 				sidepad            		= 10;       /* horizontal padding of bar */

/* Fonts */
static const char 				*fonts[]          		= { "JetBrainsMonoNL NFP:style=Bold:size=13", "Noto Sans Arabic:size=13:style=Bold","Noto Color Emoji:size=13" };
static const char 				dmenufont[]       		= "JetBrainsMonoNL NFP:style=Bold:size=13";

/* Default colors */
static const char 				col_gray1[]       		= "#222222";
static const char 				col_gray2[]       		= "#444444";
static const char 				col_gray3[]       		= "#bbbbbb";
static const char 				col_gray4[]       		= "#eeeeee";
static const char 				col_cyan[]        		= "#005577";

/* Bar icon config */ 
static const int   				myshowicon 					= 0; // Show icon menu if 1
static const int   				myiconw    					= 0;   /* 0 = auto width using TEXTW() */
static const char 				*myicon     				= "󰵆";
static const char 				*myiconcmd  				= "/home/marwa/.config/rofi/launchers/type-1/menu.sh";

/* Core Breeze Dark palette */
static const char 				col_breeze0[]  			= "#232629";  // Background
static const char 				col_breeze1[]  			= "#2A2E32";  // Alternate background
static const char 				col_breeze2[]  			= "#31363B";  // Unfocused border / dark accent
static const char 				col_breeze3[]  			= "#4D4D4D";  // Disabled text / dim gray

static const char 				col_breeze4[]  			= "#EFF0F1";  // Main foreground (text)
static const char 				col_breeze5[]  			= "#FCFCFC";  // Bright foreground (selected)
static const char 				col_breeze6[]  			= "#00BCD4";  // Cyan accent
static const char 				col_breeze7[]  			= "#3DAEE9";  // Blue highlight (KDE accent)

static const char 				col_breeze8[]  			= "#DA4453";  // Red (error)
static const char 				col_breeze9[]  			= "#F67400";  // Orange (warning)
static const char 				col_breeze10[] 			= "#C1D72E";  // Yellow/green (attention)
static const char 				col_breeze11[] 			= "#27AE60";  // Green (success)
static const char 				col_breeze12[] 			= "#9B59B6";  // Purple (special case)

/* Active colors */
static const char 				*colors[][3] 			= {
    /*							fg            		bg            	border   */
    [SchemeNorm]     			= { col_breeze4, 	col_breeze0, 	col_breeze2 }, // unfocused window
	[SchemeTitle]     			= { col_breeze5, 	col_breeze0, 	col_breeze2 }, // Window title
    [SchemeSel]      			= { col_breeze5, 	col_breeze7, 	col_breeze7 }, // focused window
    [SchemeTag]      			= { col_breeze7, 	col_breeze0, 	col_breeze2 }, // normal tag
    [SchemeTagSel]   			= { col_breeze4, 	col_breeze7, 	col_breeze5 }, // selected tag
    [SchemeTagUrg]   			= { col_breeze5, 	col_breeze8, 	col_breeze8 }, // urgent tag
    [SchemeTagEmpty]	 		= { col_breeze3, 	col_breeze0, 	col_breeze2 }, // empty/disabled tag
	[SchemeTagUnderline]    	= { col_breeze7, 	col_breeze0, 	col_breeze7 }, // blue underline
    [SchemeTagUrgUnderline] 	= { col_breeze8, 	col_breeze0, 	col_breeze8 }, // red underline
	[SchemeTagUnderlineSel] 	= { col_breeze5, 	col_breeze0, 	col_breeze5 }, // white underline (selected)
};

/* Autostart */
static const char *const 		autostart[] 			= {
    "nitrogen", "--restore", NULL,
    "picom", "--config", "/home/marwa/.config/picom/dwm.conf", NULL,
    "libinput-gestures-setup", "start", NULL,
    "setxkbmap", "-option", "grp:caps_toggle", "fr,ma", NULL,
    "lxpolkit", NULL,
    "/usr/lib/x86_64-linux-gnu/xfce4/notifyd/xfce4-notifyd", NULL,
    "xfce4-clipman", NULL,
    "nm-tray", NULL,
    "xset", "b", "off", NULL,
    "megasync", NULL,
    "slstatus", NULL,
	//"dwmblocks", NULL,
    "xfce4-screensaver", NULL,
    "xfce4-power-manager", NULL,
    "/home/marwa/.config/dwm/rkde.sh", NULL,
    NULL
};

/* Tagging */
static const char 				*tags[] 				= { "󰾔", "󰐤", "󰉌", "󰇟", "󰏬","󰎄", "󰍦", "󱃖", "" };
static const char 				*tags2[] 				= { "󰎤", "󰎧", "󰎪", "󰎭", "󰎱","󰎳", "󰎶", "󰎹", "󰎼" };

/* Underline tag*/
static const unsigned int 		ulinestroke				= 2;	/* thickness / height of the underline */
static const unsigned int 		ulinevoffset			= 0;	/* how far above the bottom of the bar the line should appear */

/* Window rules*/
static const Rule 				rules[] 				= {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      								instance    title       tags mask     isfloating   monitor */

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
	{ "io.github.celluloid_player.Celluloid",  	NULL,       NULL,       1 << 8,       0,           -1 },
	{ "mpv",  					    			NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Xed",  									NULL,       NULL,       1 << 7,       0,           -1 },
	{ "Pluma",  								NULL,       NULL,       1 << 7,       0,           -1 },
	{ "obsidian",  								NULL,       NULL,       1 << 4,       0,           -1 },
	{ "TelegramDesktop",    					NULL,       NULL,       1 << 6,       0,           -1 },
	{ "Slack",    								NULL,       NULL,       1 << 6,       0,           -1 },
};

/* Window scaling */
static const float 				mfact     				= 0.50; /* factor of master area size [0.05..0.95] */
static const int 				nmaster     			= 1;    /* number of clients in master area */
static const int 				resizehints 			= 0;    /* 1 means respect size hints in tiled resizals */
static const int 				lockfullscreen 			= 1; /* 1 will force focus on the fullscreen window */

/* Layouts */
static const Layout 		layouts[] 					= {
	/* symbol     arrange function */
	{ "󰯴",      dwindle },   /* first entry is default */
	{ "󰰤",      tile },    
	{ "󰰏",      monocle },
	{ "󰰰",      NULL },    /* no layout function means floating behavior */
	{ "󰰡",      spiral },
};

/* Commands */
static char 					dmenumon[2] 			= "0"; /* component of dmenucmd, manipulated in spawn() */
static const char 				*dmenucmd[] 			= { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char 				*termcmd[]  			= { "xfce4-terminal", NULL };

/* Keys combinitions */
static const Key 				keys[] 					= {
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
	{ MODKEY|ShiftMask,             XK_h,      rotatestack,     {.i = -1 } },
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
	{ MODKEY,             			XK_y,      spawn,          	SHCMD("~/.config/dwm/switch.sh") },
	{ MODKEY,             			XK_b,      spawn,          	SHCMD("obsidian") },
	{ 0,             				XK_Print,  spawn,          	SHCMD("flameshot gui") },
	{ MODKEY,             			XK_a,      spawn,          	SHCMD("~/.config/bspwm/pkde.py") },
	{ MODKEY,             			XK_z,      spawn,          	SHCMD("~/.config/bspwm/pemoji/pemoji.py") },
	{ MODKEY,             			XK_o,      spawn,          	SHCMD("~/.config/bspwm/blue.py") },
	{ MODKEY,             			XK_v,      spawn,          	SHCMD("~/.config/bspwm/yt.sh") },
	{ MODKEY,             			XK_x,      spawn,          	SHCMD("xed") },
	{ MODKEY,             			XK_u,      spawn,          	SHCMD("code --password-store=kwallet6") },
	{ 0,			XF86XK_AudioLowerVolume,   spawn,			SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
	{ 0,			XF86XK_AudioRaiseVolume,   spawn, 			SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
	{ 0,			XF86XK_MonBrightnessUp,    spawn,			SHCMD("~/.config/openbox/scripts/brightness.sh 0.1") },
	{ 0,			XF86XK_MonBrightnessDown,  spawn,			SHCMD("~/.config/openbox/scripts/brightness.sh -0.1") },
	{ 0,			XF86XK_AudioMute,          spawn,			SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },
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
};

/* Button definitions */
// click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin
static const Button 			buttons[] 				= {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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
