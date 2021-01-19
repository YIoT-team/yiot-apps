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

    json.user = user;
    json.pass = pass;

    pcController.sendCommand(mac, JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function setNetworkParams(mac, interface, isStatic, ip, gateway, dns, mask) {
    console.log("Set network parameters:")
    console.log("    interface: ", interface)

    let json = {}

    json.interface = interface;
    json.isStatic = isStatic;
    json.ip = ip;
    json.gateway = gateway;
    json.dns = dns;
    json.mask = mask;

    if (isStatic) {
        console.log("    type   : static")
        console.log("    ip     :", ip)
        console.log("    gateway:", gateway)
        console.log("    dns    :", dns)
        console.log("    mask   :", mask)
    } else {
        console.log("    type: dhcp")
    }

    pcController.sendCommand(mac, JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function setupAccessPoint(mac, ssid, mode, password) {
    console.log("Setup access point:")
    console.log("    ssid: ", ssid)
    console.log("    mode: ", mode)

    let json = {}

    json.ssid = ssid;
    json.mode = mode;
    json.password = password;

    pcController.sendCommand(mac, JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function setupVPNRouter(mac, vpnProvider, user, password) {
    console.log("Setup VPN router:")
    console.log("    provider: ", vpnProvider)
    console.log("    user    : ", user)

    let json = {}

    json.vpnProvider = vpnProvider;
    json.user = user;
    json.password = password;

    pcController.sendCommand(mac, JSON.stringify(json))
}

//-----------------------------------------------------------------------------
