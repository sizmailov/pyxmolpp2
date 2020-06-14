import numpy as np
import pytest


def Y2m2(rs):
    result = np.zeros(len(rs), dtype=complex)
    for i, r in enumerate(rs):
        r = r / np.linalg.norm(r)
        coef = 1.0 / 4.0 * np.sqrt(15.0 / 2.0 / np.pi)
        result[i] = coef * (r[0] - 1j * r[1]) ** 2
    return result


def Y2m1(rs):
    result = np.zeros(len(rs), dtype=complex)
    for i, r in enumerate(rs):
        r = r / np.linalg.norm(r)
        coef = 1.0 / 2.0 * np.sqrt(15.0 / 2.0 / np.pi)
        result[i] = coef * (r[0] - 1j * r[1]) * r[2]
    return result


def Y20(rs):
    result = np.zeros(len(rs), dtype=complex)
    for i, r in enumerate(rs):
        r = r / np.linalg.norm(r)

        coef = 1.0 / 4.0 * np.sqrt(5.0 / np.pi)
        result[i] = coef * (2.0 * r[2] ** 2 - r[0] ** 2 - r[1] ** 2)
    return result


def Y2p1(rs):
    result = np.zeros(len(rs), dtype=complex)
    for i, r in enumerate(rs):
        r = r / np.linalg.norm(r)
        coef = -1.0 / 2.0 * np.sqrt(15.0 / 2.0 / np.pi)
        result[i] = coef * (r[0] + 1j * r[1]) * r[2]
    return result


def Y2p2(rs):
    result = np.zeros(len(rs), dtype=complex)
    for i, r in enumerate(rs):
        r = r / np.linalg.norm(r)
        coef = 1.0 / 4.0 * np.sqrt(15.0 / 2.0 / np.pi)
        result[i] = coef * (r[0] + 1j * r[1]) ** 2
    return result


def autocorr(x):
    # return real part of autocorrelation function
    f = np.fft.fft(np.pad(x, len(x), mode='constant'))
    result = np.fft.ifft(f * np.conj(f))
    result = result[:len(x)]
    result /= np.linspace(len(x), 1, len(x))
    return np.real(result)


def autocorr_all_harmonics(r):
    res = np.array([k * autocorr(x) for x, k in zip([Y2p2(r), Y2p1(r), Y20(r)], [2, 2, 1])])
    return 0.8 * np.pi * np.sum(res, axis=0)
    # return np.sum(res, axis=0)


def autocorr_all_harmonics5(r):
    res = np.array([autocorr(x) for x in [Y2m2(r), Y2m1(r), Y2p2(r), Y2p1(r), Y20(r)]])
    return 0.8 * np.pi * np.sum(res, axis=0)


def test_autocorr():
    from pyxmolpp2 import calc_autocorr_order_2
    from numpy.random import random
    from timeit import default_timer as timer

    n = 1000
    v = np.random.random((n, 3))

    start = timer()
    cpp = calc_autocorr_order_2(v)
    end = timer()

    cpp_time = end - start

    start = timer()
    py = autocorr_all_harmonics(v)
    end = timer()
    py_time = end - start
    py2 = autocorr_all_harmonics5(v)

    assert np.max(np.abs([a - b for a, b in zip(cpp, py)])) < 1e-9
    assert np.max(np.abs([a - b for a, b in zip(py2, py)])) < 1e-9
    assert pytest.approx(1) == cpp[0]
    print("Time(c++) = %f" % cpp_time)
    print("Time(py)  = %f" % py_time)
    print("Speedup   = %g%%" % (py_time / cpp_time * 100))
    # assert py_time < cpp_time, "Python occasionally faster"
