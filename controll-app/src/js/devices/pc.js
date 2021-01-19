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
function createUser(mac, user, pass) {
    console.log("Create User: ", user)

    let json = {}

    json.command = "script"
    json.script = "create-user.sh"
    json.params = [user, pass]

    pcController.sendCommand(mac, JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function setNetworkParams(mac, interface, isStatic, ip, gateway, dns, mask) {
    console.log("Set network parameters:")
    console.log("    interface: ", interface)

    let json = {}

    json.command = "script"
    json.script = "set-network-params.sh"
    json.params = [interface, isStatic, ip, gateway, dns, mask]

    console.log("    interface : ", interface)
    if (isStatic === "true") {
        console.log("    type      : static")
        console.log("    ip        :", ip)
        console.log("    gateway   :", gateway)
        console.log("    dns       :", dns)
        console.log("    mask      :", mask)
    } else {
        console.log("    type      : dhcp")
    }

    pcController.sendCommand(mac, JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function setupAccessPoint(mac, ssid, mode, password) {
    console.log("Setup access point:")
    console.log("    ssid: ", ssid)
    console.log("    mode: ", mode)

    let json = {}

    json.command = "script"
    json.script = "setup-access-point.sh"
    json.params = [ssid, mode, password]

    pcController.sendCommand(mac, JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function setupVPNRouter(mac, vpnProvider, user, password) {
    console.log("Setup VPN router:")
    console.log("    provider: ", vpnProvider)
    console.log("    user    : ", user)

    let json = {}

    json.command = "script"
    json.script = "setup-vpn-router.sh"
    json.params = [vpnProvider, user, password]

    pcController.sendCommand(mac, JSON.stringify(json))
}

//-----------------------------------------------------------------------------
