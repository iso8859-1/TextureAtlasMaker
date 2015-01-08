#include <QTest>

class SimpleTest : public QObject
{
    Q_OBJECT
private slots:
    void SimpleTestMethod() { QVERIFY(true);}
};


QTEST_MAIN(SimpleTest)
#include "simpletest.moc"
