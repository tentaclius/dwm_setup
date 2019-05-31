/* See LICENSE file for copyright and license details. */

#define APP_CACHE "~/.cache/applications.cache"
#define APP_CACHE_SEPARATOR '/'
#define MAX_TAGLEN 16

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static char tags[][MAX_TAGLEN] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static unsigned cpt = 2; /* clients per tag (in stack area) */

static const Rule rules[0];
#if 0
= {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	//{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	//{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};
#endif

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
   { "HHH",      grid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TXT,TAG) \
	{ MODKEY,                       KEY,      view,           "tag-view " TXT, {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     "tag-toggle " TXT, {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            "win-tag-set " TXT, {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      "win-tag-toggle " TXT, {.ui = 1 << TAG} }, \
   { MODKEY|Mod1Mask,              KEY,      focusnth,       "focus-nth " TXT, {.ui = TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-p", "shell command>", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", "-e", "tmux", NULL };
//static const char *termcmd[]  = { "st", NULL };
static const char *lockscreencmd[] = { "gnome-screensaver-command", "-l", NULL };
static const char *volumeincr[] = { "volume_up", NULL };
static const char *volumedecr[] = { "volume_down", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
   { MODKEY,                       XK_apostrophe, findwin,    "win-find", {0} },
   { MODKEY|Mod1Mask,              XK_apostrophe, findwinontag, "win-find-on-tag", {0} },
   { MODKEY|ShiftMask,             XK_apostrophe, pullwin,    "win-pull", {0} },
   { MODKEY|ShiftMask,             XK_f,      findcurwin,     "win-find-first-tag", {0} },
   { MODKEY|ControlMask,           XK_l,      spawn,          "screen-lock", {.v = lockscreencmd } },
   { MODKEY,                       XK_comma,  nexttag,        "tag-prev", {.i = -1 } },
   { MODKEY,                       XK_period, nexttag,        "tag-next", {.i = +1 } },
   { MODKEY|ShiftMask,             XK_period, tonexttag,      "win-to-next-tag", {.i = +1 } },
   { MODKEY|ShiftMask,             XK_comma,  tonexttag,      "win-to-prev-tag", {.i = -1 } },
   { MODKEY,                       XK_n,      nametag,        "tag-rename", {0} },
   { MODKEY,                       XK_minus,  spawn,          "volume-decr", {.v = volumedecr } },
   { MODKEY,                       XK_equal,  spawn,          "volume-incr", {.v = volumeincr } },
   { Mod1Mask,                     XK_Tab,    prevwin,        "win-focus-prev", {0} },
   { MODKEY|Mod1Mask,              XK_h,      climit,         "layout-stack-incr", {.i = +1 } },
   { MODKEY|Mod1Mask,              XK_l,      climit,         "layout-stack-decr", {.i = -1 } },
   { MODKEY|Mod1Mask,              XK_slash,  climit,         "layout-stack-inf", {.i = 0 } },
   { MODKEY,                       XK_p,      run_app,        "run-app", {0} },
   { MODKEY,                       XK_x,      runcmd,         "", {0} },
   { MODKEY|ShiftMask,             XK_j,      rotatestack,    "rot-down", {.i = 1} },
   { MODKEY|ShiftMask,             XK_k,      rotatestack,    "rot-up", {.i = -1} },
   { MODKEY,                       XK_w,      cliuntag,       "untag", {0} },

	{ MODKEY,                       XK_r,      spawn,          "run-cmd", {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          "run-term", {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      "bar-toggle", {0} },
	{ MODKEY,                       XK_j,      focusstack,     "win-focus-next", {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     "win-focus-prev", {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      incnmaster,     "layout-master-incr", {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,      incnmaster,     "layout-master-decr", {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       "layout-factor-decr", {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       "layout-factor-incr", {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           "zoom", {0} },
	{ MODKEY,                       XK_Tab,    view,           "tag-prev", {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     "client-kill", {0} },
	{ MODKEY,                       XK_t,      setlayout,      "layout-tile", {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      "layout-monocle", {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      "layout-floating", {.v = &layouts[2]} },
   { MODKEY,                       XK_g,      setlayout,      "layout-grid", {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      "layout-toggle", {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, "win-float-toggle", {0} },
	{ MODKEY,                       XK_0,      view,           "tag-view 0", {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            "win-tag-set 0", {.ui = ~0 } },
   { MODKEY|Mod1Mask,              XK_0,      focusnth,       "focus-nth 9", {.ui = 9 } },
	//{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	//{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	//{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	//{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      "1", 0)
	TAGKEYS(                        XK_2,                      "2", 1)
	TAGKEYS(                        XK_3,                      "3", 2)
	TAGKEYS(                        XK_4,                      "4", 3)
	TAGKEYS(                        XK_5,                      "5", 4)
	TAGKEYS(                        XK_6,                      "6", 5)
	TAGKEYS(                        XK_7,                      "7", 6)
	TAGKEYS(                        XK_8,                      "8", 7)
	TAGKEYS(                        XK_9,                      "9", 8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           "quit", {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

