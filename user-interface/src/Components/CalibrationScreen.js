import React from "react";
import Header from "./Header";
import { Link } from "react-router-dom";
import Button from '@mui/material/Button';

export default function CalibrationScreen() {
    return(
        <div>
            <Header></Header>
            <h1>Calibration</h1>
            <Link to="/workout">
                <Button variant="contained">
                    <span>Begin Workout</span>
                </Button>
            </Link>
        </div>
    );
};
