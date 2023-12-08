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

const kPrepareCommand = "fw-prepare.sh"
const kVerifyCommand = "fw-verify.sh"
const kUpgradeCommand = "fw-upgrade.sh"

var fwInf = []

//-----------------------------------------------------------------------------
function
prepare(pc) {
    console.log("Request: Prepare Firmware upgrade ")

    let json = {}

    json.command = "script"
    json.script = kPrepareCommand

    pc.invokeCommand(JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function
verify(pc) {
    console.log("Request: Firmware verify ")

    let json = {}

    json.command = "script"
    json.script = kVerifyCommand

    pc.invokeCommand(JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function
upgrade(pc) {
    console.log("Request: Firmware upgrade ")

    let json = {}

    json.command = "script"
    json.script = kUpgradeCommand

    pc.invokeCommand(JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function
getInfo() {
    return fwInf
}

// ----------------------------------------------------------------------------
function
parsePrepareResult(jsObj, pc, jsonData) {
    try {
        // Check command
        if (jsonData.command !== kPrepareCommand) {
            return false
        }

        if (jsonData.success === "true") {
            jsObj.filePrepareDone()
        } else {
            jsObj.filePrepareError()
        }
    } catch (e) {
        return false
    }

    return true
}

// ----------------------------------------------------------------------------
function
parseVerificationResult(jsObj, pc, jsonData) {
    try {
        // Check command
        if (jsonData.command !== kVerifyCommand) {
            return false
        }

        fwInf = [
                    {
                        name: qsTr("Version"),
                        value: jsonData.build
                    }
                ]

        if (jsonData.success === "true") {
            jsObj.firmwareVerificationDone()
        } else {
            jsObj.firmwareVerificationError()
        }
    } catch (e) {
        return false
    }

    return true
}

// ----------------------------------------------------------------------------
