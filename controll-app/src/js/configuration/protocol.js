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

const kPrepareBackupCommand = "config-backup-prepare.sh"
const kPrepareRestoreCommand = "config-restore-prepare.sh"
const kRestoreCommand = "config-restore.sh"
const kFactoryResetCommand = "factory-reset.sh"

//-----------------------------------------------------------------------------
function
prepareBackup(pc) {
    console.log("Request: Prepare Config backup ")

    let json = {}

    json.command = "script"
    json.script = kPrepareBackupCommand

    pc.invokeCommand(JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function
prepareRestore(pc) {
    console.log("Request: Prepare Config restore ")

    let json = {}

    json.command = "script"
    json.script = kPrepareRestoreCommand

    pc.invokeCommand(JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function
restore(pc) {
    console.log("Request: Config restore ")

    let json = {}

    json.command = "script"
    json.script = kRestoreCommand

    pc.invokeCommand(JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function
factoryReset(pc, saveLevel0, saveLevel1, saveLevel2) {
    console.log("Request: Factory reset ")

    let json = {}

    json.command = "script"
    json.script = kFactoryResetCommand
    json.params = [saveLevel0, saveLevel1, saveLevel2]

    pc.invokeCommand(JSON.stringify(json))
}

// ----------------------------------------------------------------------------
function
parsePrepareBackupResult(jsObj, pc, jsonData) {
    try {
        // Check command
        if (jsonData.command !== kPrepareBackupCommand) {
            return false
        }

        if (jsonData.success === "true") {
            jsObj.backupPrepareDone()
        } else {
            jsObj.backupPrepareError()
        }
    } catch (e) {
        return false
    }

    return true
}

// ----------------------------------------------------------------------------
function
parsePrepareRestoreResult(jsObj, pc, jsonData) {
    try {
        // Check command
        if (jsonData.command !== kPrepareRestoreCommand) {
            return false
        }

        if (jsonData.success === "true") {
            jsObj.configPrepareDone()
        } else {
            jsObj.configPrepareError()
        }
    } catch (e) {
        return false
    }

    return true
}

// ----------------------------------------------------------------------------
function
parseRestoreResult(jsObj, pc, jsonData) {
    try {
        // Check command
        if (jsonData.command !== kRestoreCommand) {
            return false
        }

        if (jsonData.success === "true") {
            jsObj.configRestoreDone()
        } else {
            jsObj.configRestoreError()
        }
    } catch (e) {
        return false
    }

    return true
}

// ----------------------------------------------------------------------------
