
const DaBus = {
    processing: false,
    busline: [],
    hooks: {},
    singleton: this,
    init: function init() {
    },
    hook: function hook(what,how,to) {
        var newObject = {"how":how,"to":to};
        if( DaBus.hooks[what] === undefined ) {
            DaBus.hooks[what] = [newObject];
        } else {
            DaBus.hooks[what].push(newObject);
        }
    },
    unhook: function unhook(what,to) {
        if( Dabus.hooks[what] === undefined ) return;
        var indexToDitch = -1;
        DaBus.hooks[what].forEach( (aHook, index) => { if(aHook.how === how) indexToDitch = index; } );
        if( indexToDitch !== -1 ) DaBus.hooks[what].splice( indexToDitch, 1 );
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
        while( DaBus.busline.length > 0 ) {
            var thingToProcess = DaBus.busline.pop();
            var hooks = DaBus.hooks[thingToProcess.what];
            if( hooks !== undefined ) {
                hooks.forEach( (hookup) =>
                    { hookup.how(thingToProcess.what,hookup.with,hookup.to); } );
            }
        }
        DaBus.processing = false;
    }
};
