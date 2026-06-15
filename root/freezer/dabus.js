
const DaBus = {
    processing: false,
    busline: [],
    hooks: {},
    singleton: this,
    init: function init() {
    },
    hook: function hook(what,how,to) {
        var newObject = {"how":how,"to":to};
        if( hooks[what] === undefined ) {
            hooks[what] = [newObject];
        } else {
            hooks[what].push(newObject);
        }
    },
    unhook: function unhook(what,to) {
        if( hooks[what] === undefined ) return;
        var indexToDitch = -1;
        hooks[what].forEach( (aHook, index) => { if(aHook.to === to) indexToDitch = index; } );
        if( indexToDitch !== -1 ) hooks[what].splice( indexToDitch, 1 );
    },
    post: function post(what,anObject) {
        var newObject = {"what":what,"with":anObject};
        DaBus.busline.push(newObject);
        DaBus.kickProcessor();
    },
    kickProcessor: function kickProcessor() {
        if( DaBus.processing === false ) {
            DaBus.processing = true;
            DaBus.process();
        }
    },
    process: async function process() {
        while( busline.length > 0 ) {
            var thingToProcess = busline.pop();
            var hooks = DaBus.hooks[thingToProcess.what];
            if( hooks !== undefined ) {
                hooks.forEach( (hookup) => { hookup.to[hookup.how](hookup.with); } );
            }
        }
        DaBus.processing = false;
    }
};
