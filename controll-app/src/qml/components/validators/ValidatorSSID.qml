import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

RegularExpressionValidator {
    regularExpression: /[a-zA-Z0-9]*[-_]*[a-zA-Z0-9]+/g
}
