const ServerApi = {

"googlelogin": function googlelogin(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/googlelogin",{credentials:"same-origin",method:"ANY"} ).then( response => { DaBus.post("googlelogin", response ); } );
    } else {
        fetch( "/freezer/googlelogin",{credentials:"same-origin",method:"ANY",body:thing} ).then( response => { DaBus.post("googlelogin", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_logbody": function api_logbody(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/logbody",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_logbody", response ); } );
    } else {
        fetch( "/freezer/api/logbody",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_logbody", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_putimage": function api_product_putimage(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/product/putimage/" + what,{credentials:"same-origin",method:"POST"} ).then( DaBus.post("api_product_putimage", response) );
    } else {
        fetch( "/freezer/api/product/putimage/" + what,{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_product_putimage", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_putinfo": function api_product_putinfo(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/product/putinfo/" + what,{credentials:"same-origin",method:"POST"} ).then( DaBus.post("api_product_putinfo", response) );
    } else {
        fetch( "/freezer/api/product/putinfo/" + what,{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_product_putinfo", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_putnutrition": function api_product_putnutrition(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/product/putnutrition/" + what,{credentials:"same-origin",method:"POST"} ).then( DaBus.post("api_product_putnutrition", response) );
    } else {
        fetch( "/freezer/api/product/putnutrition/" + what,{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_product_putnutrition", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_deleteimage": function api_product_deleteimage(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/product/deleteimage/" + what,{credentials:"same-origin",method:"POST"} ).then( DaBus.post("api_product_deleteimage", response) );
    } else {
        fetch( "/freezer/api/product/deleteimage/" + what,{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_product_deleteimage", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_deleteinfo": function api_product_deleteinfo(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/product/deleteinfo/" + what,{credentials:"same-origin",method:"POST"} ).then( DaBus.post("api_product_deleteinfo", response) );
    } else {
        fetch( "/freezer/api/product/deleteinfo/" + what,{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_product_deleteinfo", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_deletenutrition": function api_product_deletenutrition(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/product/deletenutrition/" + what,{credentials:"same-origin",method:"POST"} ).then( DaBus.post("api_product_deletenutrition", response) );
    } else {
        fetch( "/freezer/api/product/deletenutrition/" + what,{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_product_deletenutrition", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_get": function api_product_get(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/product/get/" + what,{credentials:"same-origin",method:"GET"} ).then( DaBus.post("api_product_get", response) );
    } else {
        fetch( "/freezer/api/product/get/" + what,{credentials:"same-origin",method:"GET",body:thing} ).then( response => { DaBus.post("api_product_get", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_product_delete": function api_product_delete(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/product/delete/" + what,{credentials:"same-origin",method:"POST"} ).then( DaBus.post("api_product_delete", response) );
    } else {
        fetch( "/freezer/api/product/delete/" + what,{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_product_delete", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_message_get": function api_message_get(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/message/get",{credentials:"same-origin",method:"GET"} ).then( response => { DaBus.post("api_message_get", response ); } );
    } else {
        fetch( "/freezer/api/message/get",{credentials:"same-origin",method:"GET",body:thing} ).then( response => { DaBus.post("api_message_get", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_message_delete": function api_message_delete(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/message/delete",{credentials:"same-origin",method:"DELETE"} ).then( response => { DaBus.post("api_message_delete", response ); } );
    } else {
        fetch( "/freezer/api/message/delete",{credentials:"same-origin",method:"DELETE",body:thing} ).then( response => { DaBus.post("api_message_delete", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_message_accept": function api_message_accept(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/message/accept",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_message_accept", response ); } );
    } else {
        fetch( "/freezer/api/message/accept",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_message_accept", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_message_send": function api_message_send(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/message/send",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_message_send", response ); } );
    } else {
        fetch( "/freezer/api/message/send",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_message_send", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_family_get": function api_family_get(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/family/get/" + what,{credentials:"same-origin",method:"GET"} ).then( DaBus.post("api_family_get", response) );
    } else {
        fetch( "/freezer/api/family/get/" + what,{credentials:"same-origin",method:"GET",body:thing} ).then( response => { DaBus.post("api_family_get", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_family_invite": function api_family_invite(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/family/invite",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_family_invite", response ); } );
    } else {
        fetch( "/freezer/api/family/invite",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_family_invite", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_family_remove": function api_family_remove(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/family/remove",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_family_remove", response ); } );
    } else {
        fetch( "/freezer/api/family/remove",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_family_remove", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_item_add": function api_item_add(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/item/add/" + what,{credentials:"same-origin",method:"POST"} ).then( DaBus.post("api_item_add", response) );
    } else {
        fetch( "/freezer/api/item/add/" + what,{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_item_add", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_item_remove": function api_item_remove(what,thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/item/remove/" + what,{credentials:"same-origin",method:"POST"} ).then( DaBus.post("api_item_remove", response) );
    } else {
        fetch( "/freezer/api/item/remove/" + what,{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_item_remove", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_section_add": function api_section_add(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/section/add",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_section_add", response ); } );
    } else {
        fetch( "/freezer/api/section/add",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_section_add", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_section_list": function api_section_list(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/section/list",{credentials:"same-origin",method:"GET"} ).then( response => { DaBus.post("api_section_list", response ); } );
    } else {
        fetch( "/freezer/api/section/list",{credentials:"same-origin",method:"GET",body:thing} ).then( response => { DaBus.post("api_section_list", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_section_remove": function api_section_remove(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/section/remove",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_section_remove", response ); } );
    } else {
        fetch( "/freezer/api/section/remove",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_section_remove", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_section_switch": function api_section_switch(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/section/switch",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_section_switch", response ); } );
    } else {
        fetch( "/freezer/api/section/switch",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_section_switch", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_section_rename": function api_section_rename(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/section/rename",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_section_rename", response ); } );
    } else {
        fetch( "/freezer/api/section/rename",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_section_rename", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_freezer_list": function api_freezer_list(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/freezer/list",{credentials:"same-origin",method:"GET"} ).then( response => { DaBus.post("api_freezer_list", response ); } );
    } else {
        fetch( "/freezer/api/freezer/list",{credentials:"same-origin",method:"GET",body:thing} ).then( response => { DaBus.post("api_freezer_list", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_freezer_switch": function api_freezer_switch(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/freezer/switch",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_freezer_switch", response ); } );
    } else {
        fetch( "/freezer/api/freezer/switch",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_freezer_switch", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_freezer_remove": function api_freezer_remove(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/freezer/remove",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_freezer_remove", response ); } );
    } else {
        fetch( "/freezer/api/freezer/remove",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_freezer_remove", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_freezer_add": function api_freezer_add(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/freezer/add",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_freezer_add", response ); } );
    } else {
        fetch( "/freezer/api/freezer/add",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_freezer_add", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_freezer_rename": function api_freezer_rename(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/freezer/rename",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_freezer_rename", response ); } );
    } else {
        fetch( "/freezer/api/freezer/rename",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_freezer_rename", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_allow_add": function api_allow_add(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/allow/add",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_allow_add", response ); } );
    } else {
        fetch( "/freezer/api/allow/add",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_allow_add", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_allow_remove": function api_allow_remove(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/allow/remove",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_allow_remove", response ); } );
    } else {
        fetch( "/freezer/api/allow/remove",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_allow_remove", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
,
"api_allow_ban": function api_allow_ban(thing) {
let retries = 2; while (retries > 0) { try {
    if( thing === undefined ) {
        fetch( "/freezer/api/allow/ban",{credentials:"same-origin",method:"POST"} ).then( response => { DaBus.post("api_allow_ban", response ); } );
    } else {
        fetch( "/freezer/api/allow/ban",{credentials:"same-origin",method:"POST",body:thing} ).then( response => { DaBus.post("api_allow_ban", response); } );
    } } catch(e) {retries--;alert(e.message);} finally {retries = 0;};
} }
};
