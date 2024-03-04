//  ────────────────────────────────────────────────────────────
//                     ╔╗  ╔╗ ╔══╗      ╔════╗
//                     ║╚╗╔╝║ ╚╣╠╝      ║╔╗╔╗║
//                     ╚╗╚╝╔╝  ║║  ╔══╗ ╚╝║║╚╝
//                      ╚╗╔╝   ║║  ║╔╗║   ║║
//                       ║║   ╔╣╠╗ ║╚╝║   ║║
//                       ╚╝   ╚══╝ ╚══╝   ╚╝
//    ╔╗╔═╗                    ╔╗                     ╔╗
//    ║║║╔╝                   ╔╝╚╗                    ║║
//    ║╚╝╝  ╔══╗ ╔══╗ ╔══╗  ╔╗╚╗╔╝  ╔══╗ ╔╗ ╔╗╔╗ ╔══╗ ║║  ╔══╗
//    ║╔╗║  ║║═╣ ║║═╣ ║╔╗║  ╠╣ ║║   ║ ═╣ ╠╣ ║╚╝║ ║╔╗║ ║║  ║║═╣
//    ║║║╚╗ ║║═╣ ║║═╣ ║╚╝║  ║║ ║╚╗  ╠═ ║ ║║ ║║║║ ║╚╝║ ║╚╗ ║║═╣
//    ╚╝╚═╝ ╚══╝ ╚══╝ ║╔═╝  ╚╝ ╚═╝  ╚══╝ ╚╝ ╚╩╩╝ ║╔═╝ ╚═╝ ╚══╝
//                    ║║                         ║║
//                    ╚╝                         ╚╝
//
//    Lead Maintainer: Roman Kutashenko <kutashenko@gmail.com>
//  ────────────────────────────────────────────────────────────

//-----------------------------------------------------------------------------

.import "qrc:/js/uci/params-types.js" as ParamTypes

//
//  Parameter descriptors
//
var uciDescriptors = []

//
//  Parameter descriptor example: String
//
//{
//    name: <parameter name>,     // developer-friendly name
//    uci:  <uci parameter name>, // uci path
//    type: kUciParamString       // String type
//    validator: function         // Parameter validator
//}

//
//  Parameter descriptor example: Int
//
//{
//    name: <parameter name>,    // developer-friendly name
//    uci: <uci parameter name>, // uci path
//    type: kUciParamInt         // Int type
//    min: 0                     // Parameter minimum
//    max: 100                   // Parameter maximum
//}

//
//  Parameter descriptor example: Selectable
//
//{
//    name: <parameter name>,    // developer-friendly name
//    uci: <uci parameter name>, // uci path
//    type: kUciParamSelectable  // Selectable type
//    values: []                 // A list of possible values
//}


//
//  Parameter container
//
var uciParameters = []

// Parameter example
//var uciParam = {
//    device: var,          // Device object
//    values: {},           // Parameters synchronised with a device
//    changed: {}           // Parameters changed, but not ready to save
//    inProcessing: {}      // Parameters set, but isn't synchronised with a device yet
//}


//
//  Params config holder
//
var configHolder = {}

//
//  Callbacks
//
var cbUiUpdateFn = null
var cbChangesUpdateFn = null
var cbReadyToCommitFn = null

//-----------------------------------------------------------------------------
//
// Initialize uci parameters
//
function init(descriptors, uiUpdateFn, uiChangesUpdate, uiReadyToCommit) {
    uciDescriptors = descriptors

    cbUiUpdateFn = uiUpdateFn
    cbChangesUpdateFn = uiChangesUpdate
    cbReadyToCommitFn = uiReadyToCommit
}

//-----------------------------------------------------------------------------
//
//  Get uci parameter in device
//
function getDeviceValue(device, param) {
    // Get parameter descriptor
    var descr = getDescriptor(param)
    if (descr === null) {
        console.log("Parameter descriptor isn't found : ", param)
        return null
    }

    // Check if device already present
    var devParams = getDevice(device)

    // Check If device is resent
    if (devParams !== null) {
        if (devParams.values[param] !== undefined) {
            return devParams.values[param]
        }
    }

    return null
}

//-----------------------------------------------------------------------------
//
//  Get uci parameter
//
function getLocalValue(device, param) {
    // Get parameter descriptor
    var descr = getDescriptor(param)
    if (descr === null) {
        console.log("Parameter descriptor isn't found : ", param)
        return null
    }

    // Check if device already present
    var devParams = getDevice(device)

    // Check If device is absent
    if (devParams === null) {
        // Try to use default value for virtual parameter
        if (descr.uci === "") {
            return defaultVirtualVal(descr)
        }

        //        console.log("Device isn't found : ", device)
        return null
    }

    // Check in the local params
    if (devParams.changed[param] !== undefined) {
        return devParams.changed[param]
    }

    // Check in values
    if (devParams.values[param] !== undefined) {
        return devParams.values[param]
    }

    // Check default virtual parameters
    return defaultVirtualVal(descr)
}

//-----------------------------------------------------------------------------
//
//  Default virtual param value by descriptor
//
function defaultVirtualVal(descr) {
    // Check if param is virtual
    if (descr.uci !== "") {
        return undefined
    }

    if (descr.type === ParamTypes.kUciParamSelectable) {
        return descr.values[0].value
    } else {
        return "0"
    }
}

//-----------------------------------------------------------------------------
//
//  Set uci parameter
//
function set(device, param, value) {
    // Get parameter descriptor
    var descr = getDescriptor(param)
    if (descr === null) {
        console.log("Parameter descriptor isn't found : ", param)
        return
    }

    // Check if device already present
    var devParams = getDevice(device)

    // If device is absent, need to add it
    if (devParams === null) {
        devParams = {
            device: device,
            values: {},
            changed: {},
            inProcessing: {}
        }
        uciParameters.push(devParams)
    }

    // Set value for a future processing
    devParams.changed[param] = value

    cbChangesUpdateFn(device, changesPresent(device))
}

//-----------------------------------------------------------------------------
//
//  Prapare to save uci parameter
//
function save(device) {
    // Check if device already present
    var devParams = getDevice(device)

    // If device is absent, need to add it
    if (devParams === null) {
        return
    }

    var virtParams = {}
    Object.entries(devParams.changed).forEach(([key, value]) => {
                                                  var descr = getDescriptor(key)
                                                  if (descr !== null && descr.uci === "") {
                                                      virtParams[key] = value
                                                  } else {
                                                      devParams.inProcessing[key] = value
                                                  }
                                              });

    devParams.changed = virtParams

    console.log(">>> COMMIT PARAM : ", JSON.stringify(devParams.inProcessing))

    cbChangesUpdateFn(device, false)
    cbReadyToCommitFn(device, true)
}

//-----------------------------------------------------------------------------
//
//  Prapare to save uci parameter
//
function clearSaveList(device) {
    // Check if device already present
    var devParams = getDevice(device)

    // If device is absent, need to add it
    if (devParams === null) {
        return
    }

    devParams.inProcessing = {}
    cbReadyToCommitFn(device, false)
}

//-----------------------------------------------------------------------------
//
//  Decline single changed param
//
function declineSingleChange(device, param) {
    // Check if device already present
    var devParams = getDevice(device)

    // If device is absent, need to add it
    if (devParams === null) {
        return
    }

    // Reset UI value
    var newList = {}
    var present = false
    Object.entries(devParams.changed).forEach(([key, value]) => {
                                                  if (key !== param) {
                                                      newList[key] = value
                                                      present = true
                                                  }
                                              });


    // Set a new list
    devParams.changed = newList

    // Activate callback
    cbChangesUpdateFn(device, changesPresent(device))
}

//-----------------------------------------------------------------------------
//
//  Decline changed
//
function declineChanges(device) {
    // Check if device already present
    var devParams = getDevice(device)

    // If device is absent, need to add it
    if (devParams === null) {
        return
    }

    // Reset UI values
    var virtParams = {}
    Object.entries(devParams.changed).forEach(([key, value]) => {
                                                  var descr = getDescriptor(key)
                                                  console.log(">>> REVERT: ", key, " : ", JSON.stringify(devParams.values))
                                                  if (descr !== null && descr.uci === "") {
                                                      virtParams[key] = value
                                                  } else {
                                                      deviceValue(device, descr.uci, true, devParams.values[key])
                                                  }
                                              });


    // Clean list
    devParams.changed = virtParams

    // Activate callback
    cbChangesUpdateFn(device, false)
}

//-----------------------------------------------------------------------------
//
//  Get a list of parameters to be saved
//
function toBeSavedList(device) {
    // Find device
    var devParams = getDevice(device)
    if (devParams === null) {
        return []
    }

    // Collect all changed parameters
    var params = []
    Object.entries(devParams.inProcessing).forEach(([key, value]) => {
                                                       var descr = getDescriptor(key)
                                                       params.push(descr.uci + "=" + value)
                                                   });

    return params
}

//-----------------------------------------------------------------------------
//
//  Check if reboot will be activated on parameter save
//
function saveRequiresReboot(device) {
    // Find device
    var devParams = getDevice(device)
    if (devParams === null) {
        return []
    }


    // Check all changed parameters
    var requiresReboot = false
    Object.entries(devParams.inProcessing).forEach(([key, value]) => {
                                                       var descr = getDescriptor(key)
                                                       if (descr.reboots) {
                                                           requiresReboot = true
                                                       }
                                                   });

    return requiresReboot
}

//-----------------------------------------------------------------------------
//
//  Get a list of parameters in processing with descriptor names
//
function toBeSavedParamsNames(device) {
    // Find device
    var devParams = getDevice(device)
    if (devParams === null) {
        return {}
    }

    // Collect all changed parameters
    var params = {};
    Object.entries(devParams.inProcessing).forEach(([key, value]) => {
                                                       var descr = getDescriptor(key);
                                                       params[descr.name] = value;
                                                   });

    return params
}

//-----------------------------------------------------------------------------
//
//  Function to call on receive from device
//
function deviceValue(device, param, isOk, value) {
    if (!isOk) {
        return
    }

    // Get parameter descriptor
    var descr = null
    uciDescriptors.every((d) => {
                             if (d.uci === param) {
                                 descr = d
                                 return false
                             }
                             return true
                         });
    if (descr === null) {
        console.log("UCI parameter isn't found : ", param)
        return
    }

    // Check if device already present
    var devParams = getDevice(device)

    // If device is absent, need to add it
    if (devParams === null) {
        devParams = {
            device: device,
            values: {},
            changed: {},
            inProcessing: {}
        }
        uciParameters.push(devParams)
    }

    // Set value
    devParams.values[descr.name] = value

    // Send UI update event
    if (cbUiUpdateFn === null) {
        return
    }

    cbUiUpdateFn(device, descr.name, value)
}

//-----------------------------------------------------------------------------
//
//  Get Parameter descriptor
//
function getDescriptor(name) {
    for (var i = 0; i < uciDescriptors.length; i++) {
        if (uciDescriptors[i].name === name) {
            return uciDescriptors[i];
        }
    }

    return null;
}

//-----------------------------------------------------------------------------
//
//  Get Parameter Index by value
//
function getValueIndex(name, value) {
    var d = getDescriptor(name)

    if (d === null || d.type !== ParamTypes.kUciParamSelectable) {
        return 0
    }
    for (var i = 0; i < d.values.length; i++) {
        var v = d.values[i]
        if (v.value === value) {
            return i

        }
    }

    return 0
}

//-----------------------------------------------------------------------------
//
//  Get Parameter Value Description by value
//
function getValueDescripion(name, value) {
    var d = getDescriptor(name)

    if (d === null || d.type !== ParamTypes.kUciParamSelectable) {
        return value
    }
    for (var i = 0; i < d.values.length; i++) {
        var v = d.values[i]
        if (v.value === value) {
            return v.title
        }
    }

    return value
}

//-----------------------------------------------------------------------------
//
//  Get Parameter Value by index
//
function getValueByIndex(name, idx) {
    var d = getDescriptor(name)

    if (d === null || d.type !== ParamTypes.kUciParamSelectable) {
        return undefined
    }

    if (idx >= d.values.length) {
        return undefined
    }

    return d.values[idx].value
}

//-----------------------------------------------------------------------------
//
//  Get Device
//
function getDevice(device) {
    var devParams = null
    uciParameters.forEach((el) => {
                              if (el.device === device) {
                                  devParams = el
                                  return false
                              }
                              return true
                          });
    return devParams
}

//
//  Check if changes are present
//
function changesPresent(device) {
    // Check if device already present
    var devParams = getDevice(device)

    // If device is absent, need to add it
    if (devParams === null) {
        return false
    }

    var present = false
    Object.entries(devParams.changed).every(([key, value]) => {
                                                var descr = getDescriptor(key)
                                                if (descr !== null) {
                                                    if (descr.uci !== "") {
                                                        present = true
                                                        return false // exit loop
                                                    }
                                                }

                                                return true // keep iterating
                                            });
    return present
}

//
//  Add configurations set
//
function addConfigSet(name, data) {
    configHolder[name] = data
}

//
//  Get configurations set
//
function getConfigSet(name) {
    return configHolder[name]
}

//-----------------------------------------------------------------------------
