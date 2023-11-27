import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

RegularExpressionValidator {
    regularExpression: /(?!_)[a-zA-Z0-9!$%^&@\.]{1,19}/
}
