import QtQuick 2.12
import QtQuick.Controls 2.12

RegExpValidator {
    regExp:  /(?!_)[a-zA-Z0-9!$%^&@]{1,19}/
}