#include "ble.h"



//static struct bt_gatt_ccc_cfg vnd_ccc_cfg[BT_GATT_CCC_MAX] = {};

static ssize_t read_long_vnd(struct bt_conn *conn,
			     const struct bt_gatt_attr *attr, void *buf,
			     u16_t len, u16_t offset)
{
	const char *value = attr->user_data;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 sizeof(vnd_message));
}

static ssize_t write_long_vnd(struct bt_conn *conn,
			      const struct bt_gatt_attr *attr, const void *buf,
			      u16_t len, u16_t offset, u8_t flags)
{
	u8_t *value = attr->user_data;

	if (flags & BT_GATT_WRITE_FLAG_PREPARE) {
		return 0;
	}

	if (offset + len > sizeof(vnd_message)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	memcpy(value + offset, buf, len);

	return len;
}



static void blvl_ccc_cfg_changed(const struct bt_gatt_attr *attr,
				 u16_t value)
{
	simulate_blvl = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

static ssize_t read_blvl(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, u16_t len, u16_t offset)
{
	const char *value = attr->user_data;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 sizeof(*value));
}


/* Vendor Primary Service Declaration */
BT_GATT_SERVICE_DEFINE(vnd_svc,
	BT_GATT_PRIMARY_SERVICE(&vnd_uuid),
	BT_GATT_CHARACTERISTIC(&vnd_enc_uuid.uuid,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, read_blvl, NULL, &battery),
	BT_GATT_CCC(blvl_ccc_cfg, blvl_ccc_cfg_changed),
	
	BT_GATT_CHARACTERISTIC(&vnd_long_uuid.uuid, BT_GATT_CHRC_READ |
			       BT_GATT_CHRC_WRITE | BT_GATT_CHRC_EXT_PROP,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE |
			       BT_GATT_PERM_PREPARE_WRITE,
			       read_long_vnd, write_long_vnd, &vnd_message),
	
);



static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,
		      0x0d, 0x18, 0x0f, 0x18, 0x05, 0x18),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL,
		      0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
		      0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12),
};

static void connected(struct bt_conn *conn, u8_t err)
{
	if (err) {
		printk("Connection failed (err %u)\n", err);
	} else {
		printk("Connected\n");
	}
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);
}

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};

static void bt_ready(int err)
{
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	
	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");
}

static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Passkey for %s: %06u\n", addr, passkey);
}

static void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Pairing cancelled: %s\n", addr);
}

static struct bt_conn_auth_cb auth_cb_display = {
	.passkey_display = auth_passkey_display,
	.passkey_entry = NULL,
	.cancel = auth_cancel,
};

void bas_notify(void)
{
	if (!simulate_blvl) {
		return;
	}

	battery--;
	printk("%d",battery);
	if (battery ==  50U ) {
		/* Software eco battery charger */
		battery = 100U;
	}

	bt_gatt_notify(NULL, &vnd_svc.attrs[1], &battery, sizeof(battery));
}







void vnd_init(void){
	int err;

	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}
	
	bt_conn_cb_register(&conn_callbacks);
	bt_conn_auth_cb_register(&auth_cb_display);
	
}

void vnd_notify(u8_t st, u32_t val){
		
	if(st == 0U)
	{

		vnd_message[0] ='c';
		vnd_message[1] ='u';
		vnd_message[2] ='r';
		vnd_message[3] ='r';
		vnd_message[4] ='.';
		vnd_message[5] = ':'; 
		vnd_message[6] ='0';
		vnd_message[7] = '0';
		vnd_message[8] ='0';
		vnd_message[9] = '0';
		vnd_message[10] = '0';
		vnd_message[11] = '0';
		vnd_message[12] = '0';
		vnd_message[13] ='m';
		vnd_message[14] ='A';

		
		
	}
	else if(st == 1U){
		vnd_message[0] ='v';
		vnd_message[1] ='o';
		vnd_message[2] ='l';
		vnd_message[3] ='t';
		vnd_message[4] ='.';
		vnd_message[5] = ':'; 
		vnd_message[6] ='0';
		vnd_message[7] = '0';
		vnd_message[8] ='0';
		vnd_message[9] = '0';
		vnd_message[10] = '0';
		vnd_message[11] = '0';
		vnd_message[12] = '0';
		vnd_message[13] ='m';
		vnd_message[14] ='V';

		
	}
	else{
		vnd_message[0] ='p';
		vnd_message[1] ='w';
		vnd_message[2] ='r';
		vnd_message[3] ='.';
		vnd_message[4] =':';
		vnd_message[5] = '0'; 
		vnd_message[6] ='0';
		vnd_message[7] = '0';
		vnd_message[8] ='0';
		vnd_message[9] = '0';
		vnd_message[10] = '0';
		vnd_message[11] = '0';
		vnd_message[12] ='0';
		vnd_message[13] ='u';
		vnd_message[14] ='W';

	}
	
	/* setar valor da medicao */
		int i = 12;
		char aux[7];
		/*
		vnd_message[5] = aux[0]; 
		vnd_message[6] = aux[1]; 
		vnd_message[7] = aux[2]; 
		vnd_message[8] = aux[3]; 
		vnd_message[9] = aux[4]; 
		vnd_message[10] = aux[5]; 
		vnd_message[11] = aux[6]; 
		//vnd_message[12] = aux[7]; 
		
		*/
		
	
	
	bt_gatt_notify(NULL, &vnd_svc.attrs[2], &vnd_message, sizeof(vnd_message));
	
}
