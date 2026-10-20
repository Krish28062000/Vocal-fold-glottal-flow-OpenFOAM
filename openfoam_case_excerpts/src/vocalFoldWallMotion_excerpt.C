// EXCERPT - core of the prescribed vocal-fold wall motion (y-position of the
// moving boundary points), as documented in the project report, Appendix A.1.
// This is not a complete compilable boundary condition: class declaration,
// constructors and the surrounding point loop are omitted.
//
// Glottal opening height (mm):
//   H(t) = (Hmax + Hmin)/2 + (Hmax - Hmin)/2 * cos(2*pi*omega*t)
// Wall contour: straight inlet section (S1), circular arcs R1 = 7 mm,
// R2 = 6 mm, R3 = 7 mm, straight outlet section (S2). Arc start/end
// x-positions are 2nd-order polynomials in H, obtained by sweeping H in
// SolidWorks and fitting the coordinates in MATLAB.

    // Compute H
    scalar H = ((Hmax_ - Hmin_)/2)*cos(mathematical::twoPi*omega_*t) + (Hmax_ + Hmin_)/2; // in mm

    scalar strlin_cond1  = (0.044655 * pow(H,2) + 0.2341 * H + 27.6548)/1000;
    scalar curve1x_cond2 = (0.0206 * pow(H,2) + 0.10807 * H + 34.4653)/1000;
    scalar curve2x_cond2 = (-0.020616 * pow(H,2) - 0.10804 * H + 46.1404)/1000;
    scalar curve3x_cond2 = (-0.0447 * pow(H,2) - 0.2341 * H + 52.9509)/1000;

    // Check conditions and compute new position
    scalar new_y = 0.0;

    if (x <= strlin_cond1)                                  // S1: straight line
    {
        new_y = 10.0/1000;
    }
    else if (x >= strlin_cond1 && x <= curve1x_cond2)       // R1
    {
        new_y = ((sqrt(49 - pow(x*1000 - (0.044655 * pow(H, 2) + 0.2341 * H + 27.6548), 2)) + 3)/1000);
    }
    else if (x >= curve1x_cond2 && x <= curve2x_cond2)      // R2 (glottal constriction)
    {
        new_y = ((-sqrt(36 - pow(x*1000 - 40.302845, 2)) + (-8.8643e-14 * pow(H, 2) + H + 6))/1000);
    }
    else if (x >= curve2x_cond2 && x <= curve3x_cond2)      // R3
    {
        new_y = (((sqrt(49 - pow(x*1000 + 0.0447 * pow(H, 2) + 0.2341 * H - 52.9509, 2)) + 3))/1000);
    }
    else if (x >= curve3x_cond2)                            // S2: straight line
    {
        new_y = 0.01;
    }

    // Mirror for the lower vocal fold: keep the sign of the original point
    if (p0_[pointI].component(1) < 0.0)
    {
        new_y = -new_y;
    }

    // Assign new y-position
    ptsY[pointI] = new_y;
