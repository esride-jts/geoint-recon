// GEOINT Recon is a sample native desktop application for geospatial intelligence workflows.
// Copyright (C) 2020 Esri Deutschland GmbH
// Jan Tschada (j.tschada@esri.de)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <QtTest>

// add necessary includes here

class ReconTesting : public QObject
{
    Q_OBJECT

public:
    ReconTesting();
    ~ReconTesting();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

ReconTesting::ReconTesting()
{

}

ReconTesting::~ReconTesting()
{

}

void ReconTesting::initTestCase()
{

}

void ReconTesting::cleanupTestCase()
{

}

void ReconTesting::test_case1()
{

}

QTEST_APPLESS_MAIN(ReconTesting)

#include "tst_recontesting.moc"
