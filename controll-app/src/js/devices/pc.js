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

.pragma library

var factorialCount = 0;


//-----------------------------------------------------------------------------
function createUser(pcController, user, pass) {
    console.log("Create User: ", user)

    let json = {}

    json.user = user;
    json.pass = pass;

    pcController.sendCommand("1234567", JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function setNetworkParams(interface, isStatic, ip, gateway, dns, mask) {
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

    // pcController.sendCommand("1234567", JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function setupAccessPoint(ssid, mode, password) {
    console.log("Setup access point:")
    console.log("    ssid: ", ssid)
    console.log("    mode: ", mode)

    let json = {}

    json.ssid = ssid;
    json.mode = mode;
    json.password = password;

    // pcController.sendCommand("1234567", JSON.stringify(json))
}

//-----------------------------------------------------------------------------
function setupVPNRouter(vpnProvider, user, password) {
    console.log("Setup VPN router:")
    console.log("    provider: ", vpnProvider)
    console.log("    user    : ", user)

    let json = {}

    json.vpnProvider = vpnProvider;
    json.user = user;
    json.password = password;

    // pcController.sendCommand("1234567", JSON.stringify(json))
}

//-----------------------------------------------------------------------------
