const ServerApi = {

"googlelogin": async function googlelogin(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/googlelogin",{credentials:"same-origin",method:"ANY"} );
    } else {
        return fetch( "/freezer/googlelogin",{credentials:"same-origin",method:"ANY",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_logbody": async function api_logbody(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/logbody",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/logbody",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_putimage": async function api_product_putimage(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/product/putimage/" + what,{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/product/putimage/" + what,{credentials:"same-origin",method:"POST",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_putinfo": async function api_product_putinfo(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/product/putinfo/" + what,{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/product/putinfo/" + what,{credentials:"same-origin",method:"POST",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_putnutrition": async function api_product_putnutrition(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/product/putnutrition/" + what,{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/product/putnutrition/" + what,{credentials:"same-origin",method:"POST",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_deleteimage": async function api_product_deleteimage(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/product/deleteimage/" + what,{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/product/deleteimage/" + what,{credentials:"same-origin",method:"POST",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_deleteinfo": async function api_product_deleteinfo(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/product/deleteinfo/" + what,{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/product/deleteinfo/" + what,{credentials:"same-origin",method:"POST",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_deletenutrition": async function api_product_deletenutrition(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/product/deletenutrition/" + what,{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/product/deletenutrition/" + what,{credentials:"same-origin",method:"POST",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_get": async function api_product_get(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/product/get/" + what,{credentials:"same-origin",method:"GET"} );
    } else {
        return fetch( "/freezer/api/product/get/" + what,{credentials:"same-origin",method:"GET",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_delete": async function api_product_delete(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/product/delete/" + what,{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/product/delete/" + what,{credentials:"same-origin",method:"POST",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_message_get": async function api_message_get(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/message/get",{credentials:"same-origin",method:"GET"} );
    } else {
        return fetch( "/freezer/api/message/get",{credentials:"same-origin",method:"GET",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_message_delete": async function api_message_delete(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/message/delete",{credentials:"same-origin",method:"DELETE"} );
    } else {
        return fetch( "/freezer/api/message/delete",{credentials:"same-origin",method:"DELETE",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_message_accept": async function api_message_accept(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/message/accept",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/message/accept",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_message_send": async function api_message_send(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/message/send",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/message/send",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_family_get": async function api_family_get(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/family/get/" + what,{credentials:"same-origin",method:"GET"} );
    } else {
        return fetch( "/freezer/api/family/get/" + what,{credentials:"same-origin",method:"GET",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_family_invite": async function api_family_invite(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/family/invite",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/family/invite",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_family_remove": async function api_family_remove(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/family/remove",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/family/remove",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_item_add": async function api_item_add(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/item/add/" + what,{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/item/add/" + what,{credentials:"same-origin",method:"POST",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_item_remove": async function api_item_remove(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/item/remove/" + what,{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/item/remove/" + what,{credentials:"same-origin",method:"POST",body:thing} ); }
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_section_add": async function api_section_add(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/section/add",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/section/add",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_section_list": async function api_section_list(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/section/list",{credentials:"same-origin",method:"GET"} );
    } else {
        return fetch( "/freezer/api/section/list",{credentials:"same-origin",method:"GET",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_section_remove": async function api_section_remove(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/section/remove",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/section/remove",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_section_switch": async function api_section_switch(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/section/switch",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/section/switch",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_section_rename": async function api_section_rename(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/section/rename",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/section/rename",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_freezer_list": async function api_freezer_list(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/freezer/list",{credentials:"same-origin",method:"GET"} );
    } else {
        return fetch( "/freezer/api/freezer/list",{credentials:"same-origin",method:"GET",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_freezer_switch": async function api_freezer_switch(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/freezer/switch",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/freezer/switch",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_freezer_remove": async function api_freezer_remove(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/freezer/remove",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/freezer/remove",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_freezer_add": async function api_freezer_add(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/freezer/add",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/freezer/add",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_freezer_rename": async function api_freezer_rename(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/freezer/rename",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/freezer/rename",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_allow_add": async function api_allow_add(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/allow/add",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/allow/add",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_allow_remove": async function api_allow_remove(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/allow/remove",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/allow/remove",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_allow_ban": async function api_allow_ban(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        return fetch( "/freezer/api/allow/ban",{credentials:"same-origin",method:"POST"} );
    } else {
        return fetch( "/freezer/api/allow/ban",{credentials:"same-origin",method:"POST",body:thing} ); };
    } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
};
