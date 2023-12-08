import QtQuick 2.12
import QtQuick.Controls 2.12

RegExpValidator {
    regExp:  /(?!_)[a-zA-Z0-9_]{0,19}[a-zA-Z0-9]/
}
