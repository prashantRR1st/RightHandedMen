import React from "react";
import { Link } from "react-router-dom";
import Header from "./Header";
import Button from '@mui/material/Button';

export default function StartScreen() {
    return(
        <div>
            <Header></Header>
            <h1>Welcome to Plyo-Metrics</h1>
            <Link to="/calibrate">
                <Button variant="contained">
                    <span>Begin Calibration</span>
                </Button>
            </Link>
        </div>
    );
};
