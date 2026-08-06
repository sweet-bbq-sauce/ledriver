from pytest_embedded_idf.dut import IdfDut

def test_protocol(dut: IdfDut) -> None:
    dut.run_all_single_board_cases(group="parser")
