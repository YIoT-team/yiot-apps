import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.5

import "../theme"
import "../components/validators"

InputTextField {
    echoMode: TextField.PasswordEchoOnEdit
    placeholderText: qsTr("Password Hint")
    validator: ValidatorPassword {}
}
