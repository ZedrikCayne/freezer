const ServerApi = {

"googlelogin": function googlelogin(jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/googlelogin_uri",{method:"ANY"} ).then( response => { DaBus.post("googlelogin", response ); } );
    } else {
        fetch( "/googlelogin_uri",{method:"ANY",body:jsonObject} ).then( response => { DaBus.post("googlelogin", response); } );
    } }
,
"api_logbody": function api_logbody(jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/logbody_uri",{method:"POST"} ).then( response => { DaBus.post("api_logbody", response ); } );
    } else {
        fetch( "/api/logbody_uri",{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_logbody", response); } );
    } }
,
"api_product_putimage": function api_product_putimage(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/product/putimage_uri/" + what,{method:"GET"} ).then( DaBus.post("api_product_putimage", response) );
    } else {
        fetch( "/api/product/putimage_uri/" + what,{method:"GET",body:jsonObject} ).then( response => { DaBus.post("api_product_putimage", response); } );
    } }
,
"api_product_putinfo": function api_product_putinfo(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/product/putinfo_uri/" + what,{method:"GET"} ).then( DaBus.post("api_product_putinfo", response) );
    } else {
        fetch( "/api/product/putinfo_uri/" + what,{method:"GET",body:jsonObject} ).then( response => { DaBus.post("api_product_putinfo", response); } );
    } }
,
"api_product_putnutrition": function api_product_putnutrition(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/product/putnutrition_uri/" + what,{method:"POST"} ).then( DaBus.post("api_product_putnutrition", response) );
    } else {
        fetch( "/api/product/putnutrition_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_product_putnutrition", response); } );
    } }
,
"api_product_deleteimage": function api_product_deleteimage(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/product/deleteimage_uri/" + what,{method:"POST"} ).then( DaBus.post("api_product_deleteimage", response) );
    } else {
        fetch( "/api/product/deleteimage_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_product_deleteimage", response); } );
    } }
,
"api_product_deleteinfo": function api_product_deleteinfo(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/product/deleteinfo_uri/" + what,{method:"POST"} ).then( DaBus.post("api_product_deleteinfo", response) );
    } else {
        fetch( "/api/product/deleteinfo_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_product_deleteinfo", response); } );
    } }
,
"api_product_deletenutrition": function api_product_deletenutrition(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/product/deletenutrition_uri/" + what,{method:"POST"} ).then( DaBus.post("api_product_deletenutrition", response) );
    } else {
        fetch( "/api/product/deletenutrition_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_product_deletenutrition", response); } );
    } }
,
"api_product_get": function api_product_get(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/product/get_uri/" + what,{method:"GET"} ).then( DaBus.post("api_product_get", response) );
    } else {
        fetch( "/api/product/get_uri/" + what,{method:"GET",body:jsonObject} ).then( response => { DaBus.post("api_product_get", response); } );
    } }
,
"api_product_delete": function api_product_delete(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/product/delete_uri/" + what,{method:"POST"} ).then( DaBus.post("api_product_delete", response) );
    } else {
        fetch( "/api/product/delete_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_product_delete", response); } );
    } }
,
"api_message_get": function api_message_get(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/message/get_uri/" + what,{method:"GET"} ).then( DaBus.post("api_message_get", response) );
    } else {
        fetch( "/api/message/get_uri/" + what,{method:"GET",body:jsonObject} ).then( response => { DaBus.post("api_message_get", response); } );
    } }
,
"api_message_delete": function api_message_delete(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/message/delete_uri/" + what,{method:"DELETE"} ).then( DaBus.post("api_message_delete", response) );
    } else {
        fetch( "/api/message/delete_uri/" + what,{method:"DELETE",body:jsonObject} ).then( response => { DaBus.post("api_message_delete", response); } );
    } }
,
"api_message_accept": function api_message_accept(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/message/accept_uri/" + what,{method:"POST"} ).then( DaBus.post("api_message_accept", response) );
    } else {
        fetch( "/api/message/accept_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_message_accept", response); } );
    } }
,
"api_message_send": function api_message_send(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/message/send_uri/" + what,{method:"POST"} ).then( DaBus.post("api_message_send", response) );
    } else {
        fetch( "/api/message/send_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_message_send", response); } );
    } }
,
"api_family_get": function api_family_get(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/family/get_uri/" + what,{method:"GET"} ).then( DaBus.post("api_family_get", response) );
    } else {
        fetch( "/api/family/get_uri/" + what,{method:"GET",body:jsonObject} ).then( response => { DaBus.post("api_family_get", response); } );
    } }
,
"api_family_invite": function api_family_invite(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/family/invite_uri/" + what,{method:"POST"} ).then( DaBus.post("api_family_invite", response) );
    } else {
        fetch( "/api/family/invite_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_family_invite", response); } );
    } }
,
"api_family_remove": function api_family_remove(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/family/remove_uri/" + what,{method:"POST"} ).then( DaBus.post("api_family_remove", response) );
    } else {
        fetch( "/api/family/remove_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_family_remove", response); } );
    } }
,
"api_item_add": function api_item_add(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/item/add_uri/" + what,{method:"POST"} ).then( DaBus.post("api_item_add", response) );
    } else {
        fetch( "/api/item/add_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_item_add", response); } );
    } }
,
"api_item_remove": function api_item_remove(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/item/remove_uri/" + what,{method:"POST"} ).then( DaBus.post("api_item_remove", response) );
    } else {
        fetch( "/api/item/remove_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_item_remove", response); } );
    } }
,
"api_section_add": function api_section_add(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/section/add_uri/" + what,{method:"POST"} ).then( DaBus.post("api_section_add", response) );
    } else {
        fetch( "/api/section/add_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_section_add", response); } );
    } }
,
"api_section_remove": function api_section_remove(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/section/remove_uri/" + what,{method:"POST"} ).then( DaBus.post("api_section_remove", response) );
    } else {
        fetch( "/api/section/remove_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_section_remove", response); } );
    } }
,
"api_section_switch": function api_section_switch(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/section/switch_uri/" + what,{method:"POST"} ).then( DaBus.post("api_section_switch", response) );
    } else {
        fetch( "/api/section/switch_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_section_switch", response); } );
    } }
,
"api_freezer_switch": function api_freezer_switch(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/freezer/switch_uri/" + what,{method:"POST"} ).then( DaBus.post("api_freezer_switch", response) );
    } else {
        fetch( "/api/freezer/switch_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_freezer_switch", response); } );
    } }
,
"api_freezer_remove": function api_freezer_remove(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/freezer/remove_uri/" + what,{method:"POST"} ).then( DaBus.post("api_freezer_remove", response) );
    } else {
        fetch( "/api/freezer/remove_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_freezer_remove", response); } );
    } }
,
"api_freezer_add": function api_freezer_add(what,jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/freezer/add_uri/" + what,{method:"POST"} ).then( DaBus.post("api_freezer_add", response) );
    } else {
        fetch( "/api/freezer/add_uri/" + what,{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_freezer_add", response); } );
    } }
,
"api_allow_add": function api_allow_add(jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/allow/add_uri",{method:"POST"} ).then( response => { DaBus.post("api_allow_add", response ); } );
    } else {
        fetch( "/api/allow/add_uri",{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_allow_add", response); } );
    } }
,
"api_allow_remove": function api_allow_remove(jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/allow/remove_uri",{method:"POST"} ).then( response => { DaBus.post("api_allow_remove", response ); } );
    } else {
        fetch( "/api/allow/remove_uri",{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_allow_remove", response); } );
    } }
,
"api_allow_ban": function api_allow_ban(jsonObject) {
    if( jsonObject === undefined ) {
        fetch( "/api/allow/ban_uri",{method:"POST"} ).then( response => { DaBus.post("api_allow_ban", response ); } );
    } else {
        fetch( "/api/allow/ban_uri",{method:"POST",body:jsonObject} ).then( response => { DaBus.post("api_allow_ban", response); } );
    } }
};
