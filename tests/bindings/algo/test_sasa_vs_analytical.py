#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pytest
import numpy as np
from pyxmolpp2 import XYZ


def first_sphere_hidden_area(a, b, r1, r2):
    d = (a - b).len()

    if d > r1 + r2:
        return 0
    if r1 > d + r2:
        return 0

    if r2 > d + r1:
        return 4 * np.pi * r1 ** 2
    cos_alpha = (r1 ** 2 + d ** 2 - r2 ** 2) / (2 * r1 * d)

    s = (1.0 - cos_alpha) * 2 * np.pi * r1 ** 2
    return s


def test_sasa_two_spheres():
    from pyxmolpp2 import calc_sasa
    for samples, precision in [
        (10, 5e0),  # 5% accuracy
        (50, 1e0),  # 1% accuracy
        (250, 1e-1),  # 0.1% accuracy
        (1000, 1e-2),  # 0.01% accuracy
        (5000, 1e-3),  # 0.001% accuracy
    ]:
        for _ in range(100):
            a = XYZ(*(np.random.random(3) * 2 - 1))
            b = XYZ(*(np.random.random(3) * 2 - 1))

            r1, r2 = np.random.random(2) * 2 + 0.01

            S1 = 4 * np.pi * r1 ** 2
            S2 = 4 * np.pi * r2 ** 2
            S1_loss = first_sphere_hidden_area(a, b, r1, r2)
            S2_loss = first_sphere_hidden_area(b, a, r2, r1)

            total_area = S1 + S2
            exposed_area_exact = total_area - S1_loss - S2_loss

            sasa = calc_sasa(np.array([a.values, b.values]), np.array([r1, r2]), 0, n_samples=samples)
            exposed_area_approx = sum(sasa)

            exposed_area_percent_exact = exposed_area_exact / total_area * 100
            exposed_area_percent_approx = exposed_area_approx / total_area * 100

            assert np.isclose(exposed_area_percent_exact,
                              exposed_area_percent_approx,
                              atol=precision)


def test_sasa_three_spheres():
    from pyxmolpp2 import calc_sasa
    for samples, precision in [
        (10, 5e0),  # 5% accuracy
        (50, 1e0),  # 1% accuracy
        (250, 1e-1),  # 0.1% accuracy
        (1000, 1e-2),  # 0.01% accuracy
        (5000, 1e-3),  # 0.001% accuracy
    ]:
        for _ in range(100):
            r1, r2, r3 = np.random.random(3) * 2 + 0.01

            a = XYZ(*(np.random.random(3) * 2 - 1))
            b = XYZ(*(np.random.random(3) * 2 - 1))
            v = XYZ(*np.random.random(3))
            v /= v.len()
            c = a + v * (r1 + r3)

            S1 = 4 * np.pi * r1 ** 2
            S2 = 4 * np.pi * r2 ** 2
            S3 = 4 * np.pi * r3 ** 2

            S1_loss = first_sphere_hidden_area(a, b, r1, r2) + first_sphere_hidden_area(a, c, r1, r3)
            S2_loss = first_sphere_hidden_area(b, a, r2, r1) + first_sphere_hidden_area(b, c, r2, r3)
            S3_loss = first_sphere_hidden_area(c, a, r3, r1) + first_sphere_hidden_area(c, b, r3, r2)

            total_area = S1 + S2 + S3
            exposed_area_exact = total_area - S1_loss - S2_loss - S3_loss

            sasa = calc_sasa(np.array([a.values, b.values, c.values]), np.array([r1, r2, r3]), 0, n_samples=samples)
            exposed_area_approx = sum(sasa)

            exposed_area_percent_exact = exposed_area_exact / total_area * 100
            exposed_area_percent_approx = exposed_area_approx / total_area * 100

            assert np.isclose(exposed_area_percent_exact,
                              exposed_area_percent_approx,
                              atol=precision), (exposed_area_percent_exact,
                                                exposed_area_percent_approx,)


def test_sasa_errors():
    from pyxmolpp2 import calc_sasa
    coords = np.random.random((100, 3))
    radii = np.random.random((100,))
    indices = np.zeros(100).astype(np.intc)

    calc_sasa(coords, radii, 1.4, indices, 10)

    # coords.size()!=radii.size()
    with pytest.raises(RuntimeError):
        calc_sasa(coords[:-1], radii, 1.4, indices, 10)

    # too many indices
    with pytest.raises(RuntimeError):
        calc_sasa(coords[:-1], radii, 1.4, np.zeros(101).astype(np.intc), 10)

    # wrong type of indices
    with pytest.raises(RuntimeError):
        calc_sasa(coords[:-1], radii, 1.4, np.zeros(101).astype(int), 10)

    # wrong type of radii
    with pytest.raises(RuntimeError):
        calc_sasa(coords[:-1], radii.astype(np.half), 1.4, indices, 10)
