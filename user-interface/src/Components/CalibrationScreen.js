import React from "react";
import Header from "./Header";
import { Link } from "react-router-dom";
import Button from '@mui/material/Button';
import Modal from '@mui/material/Modal';
import Typography from '@mui/material/Typography';
import WorkoutChart from "./WorkoutChart";
import CalibrationChart from "./CalibrationChart";

const style = {
    position: 'absolute',
    top: '50%',
    left: '50%',
    transform: 'translate(-50%, -50%)',
    width: 400,
    background: '#c1d6f7',
    color: '#ffffff',
    border: '2px solid #000',
    boxShadow: 24,
    p: 4,
  };

export default function CalibrationScreen() {
    const [open, setOpen] = React.useState(true);
    const handleOpen = () => setOpen(true);
    const handleClose = () => setOpen(false);
    return(
        <div>
            <Header></Header>
            <Typography gutterBottom variant="h4" component="div">
                Calibration
            </Typography>
            <CalibrationChart></CalibrationChart>
            <Link to="/workout">
                <Button variant="contained">
                    <span>Begin Workout</span>
                </Button>
            </Link>
            <Modal
                open={open}
                onClose={handleClose}
                sx={style}
            >
                <Typography id="modal-title" variant="h6" component="h2" style={{paddingTop: '55%'}}>
                    Retrieving your profile from data generated on Earth...
                </Typography>
            </Modal>
        </div>
    );
};
