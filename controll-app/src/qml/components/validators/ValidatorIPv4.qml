import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

RegularExpressionValidator {
    regularExpression: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
}
