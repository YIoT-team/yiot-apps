import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 1.5

import "../theme"
import "../components/validators"

InputTextField {
    echoMode: TextField.PasswordEchoOnEdit
    placeholderText: qsTr("Password Hint")
    validator: ValidatorPassword {}
}
