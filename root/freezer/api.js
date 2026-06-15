const ServerApi = {

"googlelogin": function googlelogin(thing) {
    if( thing === undefined ) {
        fetch( "/googlelogin",{method:"ANY"} ).then( response => { DaBus.post("googlelogin", response ); } );
    } else {
        fetch( "/googlelogin",{method:"ANY",body:thing} ).then( response => { DaBus.post("googlelogin", response); } );
    } }
,
"api_logbody": function api_logbody(thing) {
    if( thing === undefined ) {
        fetch( "/api/logbody",{method:"POST"} ).then( response => { DaBus.post("api_logbody", response ); } );
    } else {
        fetch( "/api/logbody",{method:"POST",body:thing} ).then( response => { DaBus.post("api_logbody", response); } );
    } }
,
"api_product_putimage": function api_product_putimage(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/product/putimage/" + what,{method:"GET"} ).then( DaBus.post("api_product_putimage", response) );
    } else {
        fetch( "/api/product/putimage/" + what,{method:"GET",body:thing} ).then( response => { DaBus.post("api_product_putimage", response); } );
    } }
,
"api_product_putinfo": function api_product_putinfo(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/product/putinfo/" + what,{method:"GET"} ).then( DaBus.post("api_product_putinfo", response) );
    } else {
        fetch( "/api/product/putinfo/" + what,{method:"GET",body:thing} ).then( response => { DaBus.post("api_product_putinfo", response); } );
    } }
,
"api_product_putnutrition": function api_product_putnutrition(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/product/putnutrition/" + what,{method:"POST"} ).then( DaBus.post("api_product_putnutrition", response) );
    } else {
        fetch( "/api/product/putnutrition/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_product_putnutrition", response); } );
    } }
,
"api_product_deleteimage": function api_product_deleteimage(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/product/deleteimage/" + what,{method:"POST"} ).then( DaBus.post("api_product_deleteimage", response) );
    } else {
        fetch( "/api/product/deleteimage/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_product_deleteimage", response); } );
    } }
,
"api_product_deleteinfo": function api_product_deleteinfo(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/product/deleteinfo/" + what,{method:"POST"} ).then( DaBus.post("api_product_deleteinfo", response) );
    } else {
        fetch( "/api/product/deleteinfo/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_product_deleteinfo", response); } );
    } }
,
"api_product_deletenutrition": function api_product_deletenutrition(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/product/deletenutrition/" + what,{method:"POST"} ).then( DaBus.post("api_product_deletenutrition", response) );
    } else {
        fetch( "/api/product/deletenutrition/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_product_deletenutrition", response); } );
    } }
,
"api_product_get": function api_product_get(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/product/get/" + what,{method:"GET"} ).then( DaBus.post("api_product_get", response) );
    } else {
        fetch( "/api/product/get/" + what,{method:"GET",body:thing} ).then( response => { DaBus.post("api_product_get", response); } );
    } }
,
"api_product_delete": function api_product_delete(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/product/delete/" + what,{method:"POST"} ).then( DaBus.post("api_product_delete", response) );
    } else {
        fetch( "/api/product/delete/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_product_delete", response); } );
    } }
,
"api_message_get": function api_message_get(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/message/get/" + what,{method:"GET"} ).then( DaBus.post("api_message_get", response) );
    } else {
        fetch( "/api/message/get/" + what,{method:"GET",body:thing} ).then( response => { DaBus.post("api_message_get", response); } );
    } }
,
"api_message_delete": function api_message_delete(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/message/delete/" + what,{method:"DELETE"} ).then( DaBus.post("api_message_delete", response) );
    } else {
        fetch( "/api/message/delete/" + what,{method:"DELETE",body:thing} ).then( response => { DaBus.post("api_message_delete", response); } );
    } }
,
"api_message_accept": function api_message_accept(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/message/accept/" + what,{method:"POST"} ).then( DaBus.post("api_message_accept", response) );
    } else {
        fetch( "/api/message/accept/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_message_accept", response); } );
    } }
,
"api_message_send": function api_message_send(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/message/send/" + what,{method:"POST"} ).then( DaBus.post("api_message_send", response) );
    } else {
        fetch( "/api/message/send/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_message_send", response); } );
    } }
,
"api_family_get": function api_family_get(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/family/get/" + what,{method:"GET"} ).then( DaBus.post("api_family_get", response) );
    } else {
        fetch( "/api/family/get/" + what,{method:"GET",body:thing} ).then( response => { DaBus.post("api_family_get", response); } );
    } }
,
"api_family_invite": function api_family_invite(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/family/invite/" + what,{method:"POST"} ).then( DaBus.post("api_family_invite", response) );
    } else {
        fetch( "/api/family/invite/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_family_invite", response); } );
    } }
,
"api_family_remove": function api_family_remove(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/family/remove/" + what,{method:"POST"} ).then( DaBus.post("api_family_remove", response) );
    } else {
        fetch( "/api/family/remove/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_family_remove", response); } );
    } }
,
"api_item_add": function api_item_add(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/item/add/" + what,{method:"POST"} ).then( DaBus.post("api_item_add", response) );
    } else {
        fetch( "/api/item/add/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_item_add", response); } );
    } }
,
"api_item_remove": function api_item_remove(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/item/remove/" + what,{method:"POST"} ).then( DaBus.post("api_item_remove", response) );
    } else {
        fetch( "/api/item/remove/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_item_remove", response); } );
    } }
,
"api_section_add": function api_section_add(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/section/add/" + what,{method:"POST"} ).then( DaBus.post("api_section_add", response) );
    } else {
        fetch( "/api/section/add/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_section_add", response); } );
    } }
,
"api_section_remove": function api_section_remove(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/section/remove/" + what,{method:"POST"} ).then( DaBus.post("api_section_remove", response) );
    } else {
        fetch( "/api/section/remove/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_section_remove", response); } );
    } }
,
"api_section_switch": function api_section_switch(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/section/switch/" + what,{method:"POST"} ).then( DaBus.post("api_section_switch", response) );
    } else {
        fetch( "/api/section/switch/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_section_switch", response); } );
    } }
,
"api_freezer_switch": function api_freezer_switch(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/freezer/switch/" + what,{method:"POST"} ).then( DaBus.post("api_freezer_switch", response) );
    } else {
        fetch( "/api/freezer/switch/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_freezer_switch", response); } );
    } }
,
"api_freezer_remove": function api_freezer_remove(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/freezer/remove/" + what,{method:"POST"} ).then( DaBus.post("api_freezer_remove", response) );
    } else {
        fetch( "/api/freezer/remove/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_freezer_remove", response); } );
    } }
,
"api_freezer_add": function api_freezer_add(what,thing) {
    if( thing === undefined ) {
        fetch( "/api/freezer/add/" + what,{method:"POST"} ).then( DaBus.post("api_freezer_add", response) );
    } else {
        fetch( "/api/freezer/add/" + what,{method:"POST",body:thing} ).then( response => { DaBus.post("api_freezer_add", response); } );
    } }
,
"api_allow_add": function api_allow_add(thing) {
    if( thing === undefined ) {
        fetch( "/api/allow/add",{method:"POST"} ).then( response => { DaBus.post("api_allow_add", response ); } );
    } else {
        fetch( "/api/allow/add",{method:"POST",body:thing} ).then( response => { DaBus.post("api_allow_add", response); } );
    } }
,
"api_allow_remove": function api_allow_remove(thing) {
    if( thing === undefined ) {
        fetch( "/api/allow/remove",{method:"POST"} ).then( response => { DaBus.post("api_allow_remove", response ); } );
    } else {
        fetch( "/api/allow/remove",{method:"POST",body:thing} ).then( response => { DaBus.post("api_allow_remove", response); } );
    } }
,
"api_allow_ban": function api_allow_ban(thing) {
    if( thing === undefined ) {
        fetch( "/api/allow/ban",{method:"POST"} ).then( response => { DaBus.post("api_allow_ban", response ); } );
    } else {
        fetch( "/api/allow/ban",{method:"POST",body:thing} ).then( response => { DaBus.post("api_allow_ban", response); } );
    } }
};
