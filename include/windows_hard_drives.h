/**
 * @file windows_hard_drives.h
 * @brief Header file for the hard drives in Windows.
 */
#ifndef WINDOWS_HARD_DRIVES_H_INCLUDED
#define WINDOWS_HARD_DRIVES_H_INCLUDED

#include <windows.h>
#include <vector>
#include <string>
#include <iomanip>

/// @brief A class that holds the hard drives.
class Hard_Drives
{

private:
    DWORD drives;

    struct hard_drive
    {
        char drive_letter[4];
        std::string drive_type;
        ULARGE_INTEGER total_space_in_bytes;
        ULARGE_INTEGER free_space_in_bytes;
        ULARGE_INTEGER used_space_in_bytes;
        double used_percentage;
        bool fails_disk_space = false;
    };

public:
    std::vector<hard_drive> hard_drives;

    Hard_Drives()
    {
        drives = GetLogicalDrives();

        for (char letter = 'A'; letter <= 'Z'; letter++)
        {
            if (drives & (1 << (letter - 'A')))
            {
                hard_drive drive;
                drive.drive_letter[0] = letter;
                drive.drive_letter[1] = ':';
                drive.drive_letter[2] = '\\';
                drive.drive_letter[3] = '\0';

                UINT driveType = GetDriveType(drive.drive_letter);
                switch (driveType)
                {
                case DRIVE_UNKNOWN:
                    drive.drive_type.assign("Unknown drive type");
                    break;
                case DRIVE_NO_ROOT_DIR:
                    drive.drive_type.assign("Invalid root directory");
                    break;
                case DRIVE_REMOVABLE:
                    drive.drive_type.assign("Removable drive");
                    break;
                case DRIVE_FIXED:
                    drive.drive_type.assign("Fixed drive");
                    break;
                case DRIVE_REMOTE:
                    drive.drive_type.assign("Remote drive");
                    break;
                case DRIVE_CDROM:
                    drive.drive_type.assign("CD-ROM drive");
                    break;
                case DRIVE_RAMDISK:
                    drive.drive_type.assign("RAM disk");
                    break;
                }

                BOOL success = GetDiskFreeSpaceEx(drive.drive_letter, &drive.free_space_in_bytes, &drive.total_space_in_bytes, nullptr);
                if (success)
                {
                    drive.used_space_in_bytes.QuadPart = drive.total_space_in_bytes.QuadPart - drive.free_space_in_bytes.QuadPart;
                    drive.used_percentage = (double)drive.used_space_in_bytes.QuadPart / (double)drive.total_space_in_bytes.QuadPart * 100.0;
                }
                else
                {
                    drive.fails_disk_space = true;
                }
                hard_drives.push_back(drive);
            }
        }
        hard_drives.shrink_to_fit();
    };

    void print_table()
    {
        std::cout << std::left << std::setw(6) << "Drive" << std::setw(24) << "DriveType" << std::setw(20) << "Total space (GB)"
                  << std::setw(20) << "Free space (GB)" << std::setw(20) << "Used space (GB)" << std::setw(10) << "Usage %" << std::endl;
        std::cout << std::setfill('-') << std::setw(102) << "" << std::setfill(' ') << std::endl;
        for (const auto &drive : hard_drives)
        {
            double total_space_gb = drive.fails_disk_space ? 0 : static_cast<double>(drive.total_space_in_bytes.QuadPart) / (1024 * 1024 * 1024);
            double free_space_gb = drive.fails_disk_space ? 0 : static_cast<double>(drive.free_space_in_bytes.QuadPart) / (1024 * 1024 * 1024);
            double used_space_gb = drive.fails_disk_space ? 0 : static_cast<double>(drive.used_space_in_bytes.QuadPart) / (1024 * 1024 * 1024);
            double usage_percent = drive.fails_disk_space ? 0 : drive.used_percentage;

            if (drive.fails_disk_space)
            {
                total_space_gb = free_space_gb = used_space_gb = 0;
            }

            std::cout << std::left << std::setw(6) << drive.drive_letter << std::setw(24) << drive.drive_type << std::setw(20)
                      << std::fixed << std::setprecision(2) << total_space_gb << std::setw(20) << std::fixed << std::setprecision(2)
                      << free_space_gb << std::setw(20) << std::fixed << std::setprecision(2) << used_space_gb << std::setw(10)
                      << std::fixed << std::setprecision(2) << usage_percent << std::endl;
        }
    }
};

#endif // WINDOWS_HARD_DRIVES_H_INCLUDED