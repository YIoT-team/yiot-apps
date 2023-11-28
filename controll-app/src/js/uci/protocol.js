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

const kUciGetterCommand = "uci-getter.sh"
const kUciSetterCommand = "uci-setter.sh"
const kUciCommitCommand = "uci-commit.sh"

// ----------------------------------------------------------------------------
function
requestParams(pc, params) {
    console.log("Request UCI parameters: ", params)

    let json = {}

    json.command = "script"
    json.script = kUciGetterCommand
    json.params = params

    pc.invokeCommand(JSON.stringify(json))
}

// ----------------------------------------------------------------------------
function
saveParams(pc, params) {
    console.log("Save UCI parameters: ", params)

    let json = {}

    json.command = "script"
    json.script = kUciSetterCommand
    json.params = params

    pc.invokeCommand(JSON.stringify(json))
}

// ----------------------------------------------------------------------------
function
commitChanges(pc) {
    console.log("Commit UCI parameters")

    let json = {}

    json.command = "script"
    json.script = kUciCommitCommand

    pc.invokeCommand(JSON.stringify(json))
}

// ----------------------------------------------------------------------------
function
parseGetter(jsObj, pc, jsonData) {
    var res = true
    var deviceMacAddr = ""
    try {
        // Check command
        if (jsonData.command !== kUciGetterCommand) {
            return false
        }

        // Get MAC address of current device
        deviceMacAddr = pc.macAddr

        // Iterare over received parameters values
        Object.entries(jsonData.values).forEach(([key, value]) => {
           jsObj.params.deviceValue(deviceMacAddr, key, true, value)
        });
    } catch (e) {
        res = false
    }

    jsObj.paramsRequestDone(deviceMacAddr)
    return res
}

// ----------------------------------------------------------------------------
