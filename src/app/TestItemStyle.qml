/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

ButtonStyle {
    property color fontColor

    property url rightAlignedIconSource
    property url leftAlignedIconSource

    // 0: Test has not been run
    // 1: Test passed
    // 2: Test failed
    // 3: Test in progress
    property var colors: [
        '#000',
        '#0f0',
        '#f00',
        '#00f'
    ]

    background: TestItemBackground {
        pressed: control.pressed
    }
    label: Item {
        implicitWidth: row.implicitWidth
        implicitHeight: row.implicitHeight
        baselineOffset: row.y + text.y + text.baselineOffset

        Row {
            id: row
            anchors.left: control.text.length === 0 ? undefined : parent.left
            anchors.leftMargin: control.text.length === 0 ? 0 : textSingleton.implicitHeight
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: control.text.length === 0 ? parent.horizontalCenter : undefined

            Image {
                source: control.iconSource
                width: Math.min(sourceSize.width, height) + 10
                height: text.implicitHeight
                fillMode: Image.PreserveAspectFit
            }
            Text {
                id: text
                text: control.text
                color: colors[control.state]
                font.pixelSize: control.height * 0.25
                font.family: openSans.name
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Loader {
            //active: rightAlignedIconSource.toString().length !== 0
            active: leftAlignedIconSource.toString().length !== 0
            anchors.verticalCenter: parent.verticalCenter
            //anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: textSingleton.implicitHeight

            sourceComponent: Image {
                width: Math.min(sourceSize.width, height)
                height: text.implicitHeight
                fillMode: Image.PreserveAspectFit
                //source: rightAlignedIconSource
                source: leftAlignedIconSource
            }
        }
    }
}