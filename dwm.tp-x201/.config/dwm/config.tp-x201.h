/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* Macros and constants {{{ */

/**
 * Key Masks:
 *
 * Mod1Mask -> ALT_L
 * Mod4Mask -> SUPER_L
 */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY, KEY, view, {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask, KEY, toggleview, {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask, KEY, tag, {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY, toggletag, {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* helper for refreshing root window name (status bar) */
#define STATUSCMD(cmd) SHCMD(cmd "; /bin/pkill -SIGTRAP statusbar")

/* }}} */
/* Appearance {{{ */

enum { GRAY1, GRAY2, GRAY3, GRAY4, CYAN };
static const char COLORS[][8] = {
    [GRAY1] = "#222222",
    [GRAY2] = "#444444",
    [GRAY3] = "#bbbbbb",
    [GRAY4] = "#eeeeee",
    [CYAN] = "#005577",
};

static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int snap = 32; /* snap pixel */
static const int showbar= 1; /* 0 means no bar */
static const int topbar = 1; /* 0 means bottom bar */
static const char *fonts[] = { "terminus:size=10" };
static const char dmenufont[] = "terminus:size=10";
static const char *colors[][3] = {
    /* [scheme] = fg, bg, border */
    [SchemeNorm] = { COLORS[GRAY3], COLORS[GRAY1], COLORS[GRAY2] },
    [SchemeSel] = { COLORS[GRAY4], COLORS[CYAN], COLORS[CYAN] },
};

/* }}} */
/* Tagging {{{ */

static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class, instance, title, tags mask, isfloating, monitor */
	{ NULL, NULL, NULL, 0, 0, -1 },
	/* { "Gimp", NULL, NULL, 0, 1, -1 }, */
	/* { "Firefox", NULL, NULL, 1 << 8, 0, -1 }, */
};

/* }}} */
/* Layout(s) {{{  */

static const float mfact = 0.75; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1; /* number of clients in master area */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */

enum { LTILE, LFLOAT, LMONACLE };
static const Layout layouts[] = {
	/* symbol, arrange function */
	[LTILE] = { "[]=", tile }, /* first entry is default */
	[LFLOAT] = { "><>", NULL }, /* no layout function means floating behavior */
	[LMONACLE] = { "[M]", monocle },
};

/* }}} */
/* Commands: dmenu {{{ */

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "/usr/bin/dmenu_run", "-m", dmenumon,
    "-fn", dmenufont, /* Font */
    "-nb", COLORS[GRAY1], /* Normal background */
    "-nf", COLORS[GRAY3], /* Normal foreground */
    "-sb", COLORS[CYAN], /* Selected background */
    "-sf", COLORS[GRAY4], /* Selected foreground */
    NULL
};

/* }}} */
/* Commands: Launch terminal {{{ */

static const char *termcmd[]  = { "/usr/bin/x-terminal-emulator", NULL };

/* }}} */
/* Commands: Lock screen (xscreensaver) {{{ */

static const char *screensavercmd[] = {
    "/usr/bin/xscreensaver-command", "-lock"
};

/* }}} */
/* Commands: Screenshots (scrot) {{{ */

static const char *fullscreenshotcmd[] = {
    "/usr/bin/scrot", "~/Pictures/Screenshot_%Y%m%d%H%M%S.png", NULL
};

static const char *winscreenshotcmd[] = {
    "/usr/bin/scrot", "-u", "~/Pictures/Screenshot_%Y%m%d%H%M%S.png", NULL
};

/* }}} */
/* Keybindings {{{ */

static Key keys[] = {
	/* modifier, key, function, argument */

    /* Media Keys */
	{ 0, XF86XK_AudioRaiseVolume, spawn, STATUSCMD("pactl set-sink-volume 0 +5%") },
	{ 0, XF86XK_AudioLowerVolume, spawn, STATUSCMD("pactl set-sink-volume 0 -5%") },
	{ 0, XF86XK_AudioMute, spawn, STATUSCMD("pactl set-sink-mute 0 toggle") },
    { 0, XF86XK_ScreenSaver, spawn, { .v = screensavercmd } },
    { 0, XK_Print, spawn, { .v = fullscreenshotcmd } },
    { ShiftMask, XK_Print, spawn, { .v = winscreenshotcmd } },

	{ MODKEY, XK_Return, spawn, { .v = termcmd } },
    { ControlMask|Mod1Mask, XK_Delete, quit, {0} },
    { ControlMask|Mod1Mask, XK_l, spawn, { .v = screensavercmd } },
	{ MODKEY, XK_space, zoom, {0} },
	{ MODKEY, XK_Tab, view, {0} },
	{ MODKEY, XK_comma, focusmon, { .i = -1 } },
	{ MODKEY, XK_period, focusmon, { .i = +1 } },
	{ MODKEY, XK_p, spawn, { .v = dmenucmd } },
	{ MODKEY, XK_b, togglebar, {0} },
	{ MODKEY, XK_j, focusstack, { .i = +1 } },
	{ MODKEY, XK_k, focusstack, { .i = -1 } },
	{ MODKEY, XK_i, incnmaster, { .i = +1 } },
	{ MODKEY, XK_d, incnmaster, { .i = -1 } },
	{ MODKEY, XK_h, setmfact, { .f = -0.05 } },
	{ MODKEY, XK_l, setmfact, { .f = +0.05 } },
	{ MODKEY, XK_q, killclient, {0} },
	{ MODKEY, XK_t, setlayout, { .v = &layouts[LTILE] } },
	{ MODKEY, XK_f, setlayout, { .v = &layouts[LFLOAT] } },
	{ MODKEY, XK_m, setlayout, { .v = &layouts[LMONACLE] } },
	{ MODKEY, XK_0, view, { .ui = ~0 } },
	{ MODKEY|ShiftMask, XK_space,  togglefloating, { 0 } },
	{ MODKEY|ShiftMask, XK_0, tag, {.ui = ~0} },
	{ MODKEY|ShiftMask, XK_comma,  tagmon, { .i = -1 } },
	{ MODKEY|ShiftMask, XK_period, tagmon, { .i = +1 } },
	/*{ MODKEY|ShiftMask, XK_q, quit, {0} },*/
	TAGKEYS(XK_1, 0)
	TAGKEYS(XK_2, 1)
	TAGKEYS(XK_3, 2)
	TAGKEYS(XK_4, 3)
	TAGKEYS(XK_5, 4)
	TAGKEYS(XK_6, 5)
	TAGKEYS(XK_7, 6)
	TAGKEYS(XK_8, 7)
	TAGKEYS(XK_9, 8)
};

/* }}} */
/* Button definitions {{{ */

/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click, event mask, button, function, argument */
	{ ClkLtSymbol, 0, Button1, setlayout, {0} },
	{ ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]} },
	{ ClkWinTitle, 0, Button2, zoom, {0} },
	{ ClkStatusText, 0, Button2, spawn, {.v = termcmd } },
	{ ClkClientWin, MODKEY, Button1, movemouse, {0} },
	{ ClkClientWin, MODKEY, Button2, togglefloating, {0} },
	{ ClkClientWin, MODKEY, Button3, resizemouse, {0} },
	{ ClkTagBar, 0, Button1, view, {0} },
	{ ClkTagBar, 0, Button3, toggleview, {0} },
	{ ClkTagBar, MODKEY, Button1, tag, {0} },
	{ ClkTagBar, MODKEY, Button3, toggletag, {0} },
};

/* }}} */

