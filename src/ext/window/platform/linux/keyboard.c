#include <sll.h>
#include <stdlib.h>
#include <window/common.h>
#include <window/keyboard.h>
#include <xcb/xcb.h>
#include <xcb/xkb.h>



typedef struct _XCB_TO_WINDOW_KEY{
	const char name[4];
	window_key_t key;
} xcb_to_window_key_t;



static const xcb_to_window_key_t _key_mapping[]={
	{"BKSP",WINDOW_KEY_BACKSPACE},
	{"TAB",WINDOW_KEY_TAB},
	{"RTRN",WINDOW_KEY_ENTER},
	{"ESC",WINDOW_KEY_ESCAPE},
	{"SPCE",WINDOW_KEY_SPACE},
	{"AC11",WINDOW_KEY_APOSTROPHE},
	{"AB08",WINDOW_KEY_COMMA},
	{"AE11",WINDOW_KEY_MINUS},
	{"AB09",WINDOW_KEY_PERIOD},
	{"AB10",WINDOW_KEY_SLASH},
	{"AE10",WINDOW_KEY_0},
	{"AE01",WINDOW_KEY_1},
	{"AE02",WINDOW_KEY_2},
	{"AE03",WINDOW_KEY_3},
	{"AE04",WINDOW_KEY_4},
	{"AE05",WINDOW_KEY_5},
	{"AE06",WINDOW_KEY_6},
	{"AE07",WINDOW_KEY_7},
	{"AE08",WINDOW_KEY_8},
	{"AE09",WINDOW_KEY_9},
	{"AC10",WINDOW_KEY_SEMICOLON},
	{"AE12",WINDOW_KEY_EQUAL},
	{"AC01",WINDOW_KEY_A},
	{"AB05",WINDOW_KEY_B},
	{"AB03",WINDOW_KEY_C},
	{"AC03",WINDOW_KEY_D},
	{"AD03",WINDOW_KEY_E},
	{"AC04",WINDOW_KEY_F},
	{"AC05",WINDOW_KEY_G},
	{"AC06",WINDOW_KEY_H},
	{"AD08",WINDOW_KEY_I},
	{"AC07",WINDOW_KEY_J},
	{"AC08",WINDOW_KEY_K},
	{"AC09",WINDOW_KEY_L},
	{"AB07",WINDOW_KEY_M},
	{"AB06",WINDOW_KEY_N},
	{"AD09",WINDOW_KEY_O},
	{"AD10",WINDOW_KEY_P},
	{"AD01",WINDOW_KEY_Q},
	{"AD04",WINDOW_KEY_R},
	{"AC02",WINDOW_KEY_S},
	{"AD05",WINDOW_KEY_T},
	{"AD07",WINDOW_KEY_U},
	{"AB04",WINDOW_KEY_V},
	{"AD02",WINDOW_KEY_W},
	{"AB02",WINDOW_KEY_X},
	{"AD06",WINDOW_KEY_Y},
	{"AB01",WINDOW_KEY_Z},
	{"AD11",WINDOW_KEY_LEFT_BRACKET},
	{"BKSL",WINDOW_KEY_BACKSLASH},
	{"AD12",WINDOW_KEY_RIGHT_BRACKET},
	{"TLDE",WINDOW_KEY_GRAVE_ACCENT},
	{"LSGT",WINDOW_KEY_OTHER_1},
	{"FK01",WINDOW_KEY_F1},
	{"FK02",WINDOW_KEY_F2},
	{"FK03",WINDOW_KEY_F3},
	{"FK04",WINDOW_KEY_F4},
	{"FK05",WINDOW_KEY_F5},
	{"FK06",WINDOW_KEY_F6},
	{"FK07",WINDOW_KEY_F7},
	{"FK08",WINDOW_KEY_F8},
	{"FK09",WINDOW_KEY_F9},
	{"FK10",WINDOW_KEY_F10},
	{"FK11",WINDOW_KEY_F11},
	{"FK12",WINDOW_KEY_F12},
	{"FK13",WINDOW_KEY_F13},
	{"FK14",WINDOW_KEY_F14},
	{"FK15",WINDOW_KEY_F15},
	{"FK16",WINDOW_KEY_F16},
	{"FK17",WINDOW_KEY_F17},
	{"FK18",WINDOW_KEY_F18},
	{"FK19",WINDOW_KEY_F19},
	{"FK20",WINDOW_KEY_F20},
	{"FK21",WINDOW_KEY_F21},
	{"FK22",WINDOW_KEY_F22},
	{"FK23",WINDOW_KEY_F23},
	{"FK24",WINDOW_KEY_F24},
	{"FK25",WINDOW_KEY_F25},
	{"UP",WINDOW_KEY_UP},
	{"DOWN",WINDOW_KEY_DOWN},
	{"LEFT",WINDOW_KEY_LEFT},
	{"RGHT",WINDOW_KEY_RIGHT},
	{"INS",WINDOW_KEY_INSERT},
	{"DELE",WINDOW_KEY_DELETE},
	{"PGUP",WINDOW_KEY_PAGE_UP},
	{"PGDN",WINDOW_KEY_PAGE_DOWN},
	{"HOME",WINDOW_KEY_HOME},
	{"END",WINDOW_KEY_END},
	{"CAPS",WINDOW_KEY_CAPS_LOCK},
	{"SCLK",WINDOW_KEY_SCROLL_LOCK},
	{"NMLK",WINDOW_KEY_NUM_LOCK},
	{"PRSC",WINDOW_KEY_PRINT_SCREEN},
	{"PAUS",WINDOW_KEY_PAUSE},
	{"KP0",WINDOW_KEY_KP_0},
	{"KP1",WINDOW_KEY_KP_1},
	{"KP2",WINDOW_KEY_KP_2},
	{"KP3",WINDOW_KEY_KP_3},
	{"KP4",WINDOW_KEY_KP_4},
	{"KP5",WINDOW_KEY_KP_5},
	{"KP6",WINDOW_KEY_KP_6},
	{"KP7",WINDOW_KEY_KP_7},
	{"KP8",WINDOW_KEY_KP_8},
	{"KP9",WINDOW_KEY_KP_9},
	{"KPDL",WINDOW_KEY_KP_DECIMAL},
	{"KPDV",WINDOW_KEY_KP_DIVIDE},
	{"KPMU",WINDOW_KEY_KP_MULTIPLY},
	{"KPSU",WINDOW_KEY_KP_SUBTRACT},
	{"KPAD",WINDOW_KEY_KP_ADD},
	{"KPEN",WINDOW_KEY_KP_ENTER},
	{"KPEQ",WINDOW_KEY_KP_EQUAL},
	{"LFSH",WINDOW_KEY_LEFT_SHIFT},
	{"LCTL",WINDOW_KEY_LEFT_CONTROL},
	{"LALT",WINDOW_KEY_LEFT_ALT},
	{"LWIN",WINDOW_KEY_LEFT_SUPER},
	{"RTSH",WINDOW_KEY_RIGHT_SHIFT},
	{"RCTL",WINDOW_KEY_RIGHT_CONTROL},
	{"RALT",WINDOW_KEY_RIGHT_ALT},
	{"LVL3",WINDOW_KEY_RIGHT_ALT},
	{"MDSW",WINDOW_KEY_RIGHT_ALT},
	{"RWIN",WINDOW_KEY_RIGHT_SUPER},
	{"MENU",WINDOW_KEY_MENU}
};



window_key_t _scancode_to_keycode[256];
uint8_t _keycode_to_scancode[WINDOW_MAX_KEY+1];



static window_key_t _lookup_key(const char* name){
	uint32_t name_v=*((const uint32_t*)name);
	for (unsigned int i=0;i<sizeof(_key_mapping)/sizeof(xcb_to_window_key_t);i++){
		const xcb_to_window_key_t* mapping=_key_mapping+i;
		if (name_v==*((const uint32_t*)(&(mapping->name)))){
			return mapping->key;
		}
	}
	return WINDOW_KEY_UNKNOWN;
}



void _setup_key_mapping(void){
	free(xcb_xkb_use_extension_reply(_xcb_conn,xcb_xkb_use_extension(_xcb_conn,XCB_XKB_MAJOR_VERSION,XCB_XKB_MINOR_VERSION),NULL));
	sll_zero_memory(_scancode_to_keycode,256*sizeof(window_key_t));
	sll_zero_memory(_keycode_to_scancode,WINDOW_MAX_KEY+1);
	xcb_xkb_get_names_reply_t* names=xcb_xkb_get_names_reply(_xcb_conn,xcb_xkb_get_names(_xcb_conn,XCB_XKB_ID_USE_CORE_KBD,XCB_XKB_NAME_DETAIL_KEY_NAMES|XCB_XKB_NAME_DETAIL_KEY_ALIASES),NULL);
	xcb_xkb_get_names_value_list_t name_list;
	xcb_xkb_get_names_value_list_unpack(xcb_xkb_get_names_value_list(names),names->nTypes,names->indicators,names->virtualMods,names->groupNames,names->nKeys,names->nKeyAliases,names->nRadioGroups,names->which,&name_list);
	int max=xcb_xkb_get_names_value_list_key_names_length(names,&name_list);
	if (max>256-names->minKeyCode){
		max=256-names->minKeyCode;
	}
	int max_aliases=xcb_xkb_get_names_value_list_key_aliases_length(names,&name_list);
	xcb_xkb_key_name_iterator_t iter=xcb_xkb_get_names_value_list_key_names_iterator(names,&name_list);
	for (int i=0;i<max;i++){
		window_key_t key=_lookup_key(iter.data->name);
		if (key==WINDOW_KEY_UNKNOWN){
			uint32_t name_v=*((const uint32_t*)(iter.data->name));
			xcb_xkb_key_alias_iterator_t alias_iter=xcb_xkb_get_names_value_list_key_aliases_iterator(names,&name_list);
			for (int i=0;i<max_aliases;i++){
				if (name_v==*((const uint32_t*)(alias_iter.data->real))){
					key=_lookup_key(alias_iter.data->alias);
					if (key!=WINDOW_KEY_UNKNOWN){
						break;
					}
				}
				xcb_xkb_key_alias_next(&alias_iter);
			}
		}
		if (key!=WINDOW_KEY_UNKNOWN){
			_scancode_to_keycode[i+names->minKeyCode]=key;
		}
		xcb_xkb_key_name_next(&iter);
	}
	free(names);
}
